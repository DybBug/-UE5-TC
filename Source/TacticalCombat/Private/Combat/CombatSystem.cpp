// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/CombatSystem.h"
#include "Components/SceneComponent.h"
#include "Grid/Grid.h"
#include "Unit/Unit.h"

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

	m_Grid->OnGridGenerated.AddUObject(this, &ACombatSystem::_OnGridGenerated);
	m_Grid->OnTileDataUpdated.AddUObject(this, &ACombatSystem::_OnTileDataUpdated);
}

void ACombatSystem::AddUnitInCombat(AUnit* _pUnit, const FIntPoint& _index)
{
	m_UnitsInCombat.Add(_pUnit);
	_SetUnitIndexOnGrid(_pUnit, _index);
}

void ACombatSystem::RemoveUnitFromCombat(AUnit* _pUnit, bool _bIsUnitDestroyed)
{
	m_UnitsInCombat.Remove(_pUnit);
	_SetUnitIndexOnGrid(_pUnit, Grid::INVALID_POINT_VALUE);

	if (_bIsUnitDestroyed)
	{
		_pUnit->Destroy();
	}
}

#pragma region Privete Methods
void ACombatSystem::_SetUnitIndexOnGrid(AUnit* _pUnit, const FIntPoint& _index)
{
	const FIntPoint& unitIndexOnGird = _pUnit->GetIndex();
	if (unitIndexOnGird != _index)
	{
		const auto& gridTileMap = m_Grid->GetGridTileMap();

		if (const FTileData* pOldTile = gridTileMap.Find(unitIndexOnGird))
		{
			if (pOldTile->UnitOnTile == _pUnit)
			{
				FTileData newTile = (*pOldTile);
				newTile.UnitOnTile = nullptr;
				m_Grid->AddGridTile(newTile);
			}
		}	
		
		_pUnit->SetIndex(_index);
		if (_index != FIntPoint(Grid::INVALID_POINT_VALUE))
		{
			if (const FTileData* pTile = gridTileMap.Find(_index))
			{
				FTileData newTile = (*pTile);
				newTile.UnitOnTile = _pUnit;
				m_Grid->AddGridTile(newTile);
			}
		}
	}
	const auto& gridTileMap = m_Grid->GetGridTileMap();
	if (const FTileData* pTile = gridTileMap.Find(_index))
	{
		_pUnit->SetActorLocation(pTile->Transform.GetLocation());
	}
	else
	{
		_pUnit->SetActorLocation(FVector(Unit::INVALID_UNIT_LOCATION_VALUE));		
	}	
}

void ACombatSystem::_OnGridGenerated()
{
	for (int i = m_UnitsInCombat.Num() - 1; i >= 0; --i)
	{
		AUnit* pUnit = m_UnitsInCombat[i].Get();
		if (!pUnit) continue;

		bool shouldRemove = !m_Grid->IsWalkableTile(pUnit->GetIndex());
		if (shouldRemove)
		{
			RemoveUnitFromCombat(pUnit, true);
		}
		else
		{
			_SetUnitIndexOnGrid(pUnit, pUnit->GetIndex());
		}
	}
}

void ACombatSystem::_OnTileDataUpdated(const FIntPoint& _index)
{
	for (int i = m_UnitsInCombat.Num() - 1; i >= 0; --i)
	{
		AUnit* pUnit = m_UnitsInCombat[i].Get();
		if (pUnit->GetIndex() != _index)
			continue;
		
		bool shouldRemove = !m_Grid->IsWalkableTile(pUnit->GetIndex());
		if (shouldRemove)
		{
			RemoveUnitFromCombat(pUnit, true);
			break;
		}
		else
		{
			_SetUnitIndexOnGrid(pUnit, pUnit->GetIndex());
			break;
		}
	}
}
#pragma endregion
