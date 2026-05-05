// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/CombatSystem.h"

#include "Combat/Spells/SpellAnimation.h"
#include "Components/SceneComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Grid/Grid.h"
#include "Library/SpellLibrary.h"
#include "Unit/Unit.h"
#include "Shared/SharedDefines.h"

// Sets default values
ACombatSystem::ACombatSystem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	m_SceneComponent = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = m_SceneComponent;
}

// Called when the game starts or when spawned
void ACombatSystem::BeginPlay()
{
	Super::BeginPlay();

	m_Grid->BindGridCreated(this, &ACombatSystem::_HandleGridGenerated);
	m_Grid->BindTileDataChanged(this, &ACombatSystem::_HandleTileDataUpdated);
}

void ACombatSystem::AddUnitInCombat(AUnit* _pUnit, const FIntPoint& _index)
{
	m_UnitsInCombat.Add(_pUnit);
	_SetUnitIndexOnGridWithNotify(_pUnit, _index, false);
	_pUnit->BindUnitResearchedNewTile(this, &ACombatSystem::_HandleUnitResearchedNewTile);
}

void ACombatSystem::RemoveUnitFromCombat(AUnit* _pUnit, bool _bIsUnitDestroyed)
{
	m_UnitsInCombat.Remove(_pUnit);
	_SetUnitIndexOnGridWithNotify(_pUnit, Grid::INVALID_POINT_VALUE, false);

	if (_bIsUnitDestroyed)
	{
		_pUnit->Destroy();
	}
}

TArray<FIntPoint> ACombatSystem::GetSpellRangeIndices(const FIntPoint& _originIndex, const FSpellCast& _cast) const
{	
	TArray<FIntPoint> rangeIndices = USpellLibrary::GetIndicesFromPatternAndRange(
		_originIndex,
		m_Grid->GetGridShape(),
		_cast.RangePattern,
		_cast.RangeBounds);	

	if (_cast.bIsRequireLineOfSight)
	{
		return GetIndicesWithLineOfSight(_originIndex, rangeIndices, _cast);
	}

	return rangeIndices;
}

void ACombatSystem::CastSpell(ESpellType _spellType, const FIntPoint& _originIndex, const TArray<FIntPoint>& _targetIndices)
{
	if (_spellType == ESpellType::None)
		return;
	
	FSpellTableRow spellRowData = USpellLibrary::GetSpellRowDataFromType(_spellType);	

	const TArray<FIntPoint>& spellRangeIndices = GetSpellRangeIndices(_originIndex, spellRowData.SpellCast);
	for (const FIntPoint& targetIndex : _targetIndices)
	{
		if (spellRangeIndices.Contains(targetIndex))
		{
			ASpellAnimation* pSpellAnimation = GetWorld()->SpawnActor<ASpellAnimation>(spellRowData.SpellAsset.SpellAnimationClass);
			pSpellAnimation->PostSpawnInitialize(m_Grid.Get(), _originIndex, _targetIndices);
			break;
		}
	}
}

bool ACombatSystem::HasLineOfSight(const FIntPoint& _originIndex, const FIntPoint& _targetIndex, const FSpellCast& _cast) const
{
	const FTileData* pOriginTile =  m_Grid->GetGridTileMap().Find(_originIndex);
	if (!pOriginTile)
		return false;

	const FTileData* pTargetTile = m_Grid->GetGridTileMap().Find(_targetIndex);
	if (!pTargetTile)
		return false;

	const FVector& tileSize = m_Grid->GetTileSize();
	FVector offset = tileSize * _cast.LineOfSight.OffsetFromCenter;
	TArray<FVector> originOffsets = {
		FVector(+offset.X, 0.0f, 0.0f), FVector(0.0f, +offset.Y, 0.0f),
		FVector(-offset.X, 0.0f, 0.0f), FVector(0.0f, -offset.Y, 0.0f)
	};

	for (const FVector& originOffset : originOffsets)
	{
		const FVector& originLocation = pOriginTile->Transform.GetLocation() + FVector(0.0f, 0.0f, _cast.LineOfSight.HeightFromGround) + originOffset;
		const FVector& targetLocation = pTargetTile->Transform.GetLocation() + FVector(0.0f, 0.0f, _cast.LineOfSight.HeightFromGround);

		EDrawDebugTrace::Type debugDrawType = _cast.LineOfSight.bIsDrawDebugLine ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;
		// 라인 트레이스 결과 저장
		FHitResult hitResult;
		bool bIsHit = UKismetSystemLibrary::LineTraceSingle(
			GetWorld(),
			originLocation,
			targetLocation,
			UEngineTypes::ConvertToTraceType(ECC_LineOfSight), // ECC_Visibility에 해당하는 TraceChannel
			false, // bTraceComplex
			TArray<AActor*>(), // ActorsToIgnore
			debugDrawType, // DrawDebugType
			hitResult,
			true // bIgnoreSelf
		);

		if (!bIsHit) {
			return true;
		}
	}

	return false;
}

TArray<FIntPoint> ACombatSystem::GetIndicesWithLineOfSight(const FIntPoint& _originIndex, const TArray<FIntPoint>& _indices, const FSpellCast& _cast) const
{
	TArray<FIntPoint> indicesWithLineOfSight;
	for (const FIntPoint& index : _indices)
	{		
		// 시야가 없는 인덱스는 건너 뛰기
		if (!HasLineOfSight(_originIndex, index, _cast))
			continue;

		// 시야가 있는 인덱스는 추가
		indicesWithLineOfSight.Add(index);
	}
	return indicesWithLineOfSight;
}

#pragma region Privete Methods
void ACombatSystem::_SetUnitIndexOnGridWithNotify(AUnit* _pUnit, const FIntPoint& _index, bool _bIsForce)
{
	const FIntPoint& unitIndexOnGird = _pUnit->GetIndex();
	if (_bIsForce || unitIndexOnGird != _index)
	{
		const auto& gridTileMap = m_Grid->GetGridTileMap();
		
		if (const FTileData* pOldTile = gridTileMap.Find(unitIndexOnGird))
		{
			if (pOldTile->UnitOnTile == _pUnit)
			{
				FTileData tempOldTile = (*pOldTile);
				tempOldTile.UnitOnTile = nullptr;
				m_Grid->AddGridTileWithNotify(tempOldTile);
			}
		}			

		_pUnit->SetGridIndex(_index);

		if (_index != FIntPoint(Grid::INVALID_POINT_VALUE))
		{
			if (const FTileData* pTile = gridTileMap.Find(_index))
			{
				FTileData newTile = (*pTile);
				newTile.UnitOnTile = _pUnit;
				m_Grid->AddGridTileWithNotify(newTile);
			}
		}
		
		if (const FTileData* pTile = gridTileMap.Find(_index))
		{
			_pUnit->SetActorLocation(pTile->Transform.GetLocation());
		}
		else
		{
			_pUnit->SetActorLocation(FVector(Unit::INVALID_UNIT_LOCATION_VALUE));		
		}

		BroadcastUnitGridIndexChanged(_pUnit);
	}
}
#pragma endregion

#pragma region Event Handlers
void ACombatSystem::_HandleGridGenerated()
{
	for (int i = m_UnitsInCombat.Num() - 1; i >= 0; --i)
	{
		AUnit* pUnit = m_UnitsInCombat[i].Get();
		if (!pUnit) continue;

		const FIntPoint& index = pUnit->GetIndex();				
		bool shouldRemove = (pUnit->GetUnitData().Stats.ValidTileTypeFlags & static_cast<uint8>(m_Grid->GetGridTileMap()[index].Type)) == 0;
		if (shouldRemove)
		{
			RemoveUnitFromCombat(pUnit, true);
		}
		else
		{
			_SetUnitIndexOnGridWithNotify(pUnit, pUnit->GetIndex(), true);
		}
	}
}

void ACombatSystem::_HandleTileDataUpdated(const FIntPoint& _index)
{
	for (int i = m_UnitsInCombat.Num() - 1; i >= 0; --i)
	{
		AUnit* pUnit = m_UnitsInCombat[i].Get();
		if (pUnit->GetIndex() != _index)
			continue;

		const FIntPoint& index = pUnit->GetIndex();				
		bool shouldRemove = (pUnit->GetUnitData().Stats.ValidTileTypeFlags & static_cast<uint8>(m_Grid->GetGridTileMap()[index].Type)) == 0;
		if (shouldRemove)
		{
			RemoveUnitFromCombat(pUnit, true);
			break;
		}
		else
		{
			_SetUnitIndexOnGridWithNotify(pUnit, pUnit->GetIndex(), false);
			break;
		}
	}
}

void ACombatSystem::_HandleUnitResearchedNewTile(AUnit* const _pUnit, const FIntPoint& _index)
{
	_SetUnitIndexOnGridWithNotify(_pUnit, _index, false);
}

#pragma endregion
