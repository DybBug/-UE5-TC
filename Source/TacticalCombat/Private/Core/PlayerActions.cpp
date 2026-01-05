// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/PlayerActions.h"

#include "Action/AbstractAction.h"
#include "Combat/CombatSystem.h"
#include "Grid/Grid.h"
#include "Kismet/GameplayStatics.h"
#include "Shared/SharedDefines.h"
#include "Unit/Unit.h"

// Sets default values
APlayerActions::APlayerActions()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<UAbstractAction> leftClickSelectActionClassFinder(TEXT("/Script/TacticalCombat.SelectTileAction"));
	if (leftClickSelectActionClassFinder.Succeeded())
	{
		m_LeftClickSelectActionClass = leftClickSelectActionClassFinder.Class;
	}

	static ConstructorHelpers::FClassFinder<UAbstractAction> rightClickSelectActionClassFinder(TEXT("/Script/TacticalCombat.SelectTileAction"));
	if (rightClickSelectActionClassFinder.Succeeded())
	{
		m_RightClickSelectActionClass = rightClickSelectActionClassFinder.Class;
	}
}

// Called when the game starts or when spawned
void APlayerActions::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* pPlayerController =  GetWorld()->GetFirstPlayerController();
	if (pPlayerController)
	{
		EnableInput(pPlayerController);
		InputComponent->BindAction("LeftClick", IE_Pressed, this, &APlayerActions::_HandleLeftClickPressed);
		InputComponent->BindAction("LeftClick", IE_Released, this, &APlayerActions::_HandleLeftClickReleased);
		InputComponent->BindAction("RightClick", IE_Pressed, this, &APlayerActions::_HandleRightClickPressed);
		InputComponent->BindAction("RightClick", IE_Released, this, &APlayerActions::_HandleRightClickReleased);
	}
	
	m_Grid = Cast<AGrid>(UGameplayStatics::GetActorOfClass(GetWorld(), AGrid::StaticClass()));
	m_Grid->OnGridGenerated.AddUObject(this, &APlayerActions::_HandleGridGenerated);
	m_Grid->OnTileDataUpdated.AddUObject(this, &APlayerActions::_HandleTileDataUpdated);
	
	m_CombatSystem = Cast<ACombatSystem>(UGameplayStatics::GetActorOfClass(GetWorld(), ACombatSystem::StaticClass()));
	m_CombatSystem->OnUnitGridIndexChanged.AddUObject(this, &APlayerActions::_HandleUnitGridIndexChanged);
	
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->InputEnabled();
}

// Called every frame
void APlayerActions::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	_UpdateHoveredUnit();
	_UpdateHoveredTile();
}

void APlayerActions::SetSelectedActionWithNotify(const TSubclassOf<UAbstractAction>& _leftClickActionClass, const TSubclassOf<UAbstractAction>& _rightClickActionClass)
{
	if (m_LeftClickSelectAction)
	{		
		m_LeftClickSelectAction = nullptr;
	}
	
	if (_leftClickActionClass != nullptr)
	{		
		m_LeftClickSelectAction = NewObject<UAbstractAction>(this, _leftClickActionClass);
		m_LeftClickSelectAction->Initialize(this);
	}

	if (m_RightClickSelectAction)
	{
		m_RightClickSelectAction = nullptr;
	}

	if (_rightClickActionClass != nullptr)
	{
		m_RightClickSelectAction = NewObject<UAbstractAction>(this, _rightClickActionClass);
		m_RightClickSelectAction->Initialize(this);
	}

	if (OnSelectedActionsChanged.IsBound())
	{
		OnSelectedActionsChanged.Broadcast(m_LeftClickSelectAction, m_RightClickSelectAction);
	}
}

void APlayerActions::SelectTileAndUnit(const FIntPoint& _index, bool _isForce)
{
	// 타일 선택
	if (_isForce || m_SelectedTileIndex != _index)
	{
		m_Grid->RemoveStateFromTileWithNotify(m_SelectedTileIndex, ETileStateFlags::Selected);
		
		SetSelectedTileIndex(_index);
		m_Grid->AddStateToTileWithNotify(_index, ETileStateFlags::Selected);
	}
	else
	{
		m_Grid->RemoveStateFromTileWithNotify(m_SelectedTileIndex, ETileStateFlags::Selected);
		SetSelectedTileIndex(FIntPoint(INVALID_POINT_VALUE));
	}

	// 유닛 선택
	const FTileData* pSelectedTileData= m_Grid->GetGridTileMap().Find(m_SelectedTileIndex);
	if (_isForce || pSelectedTileData)
	{
		if (m_SelectedUnit != pSelectedTileData->UnitOnTile)
		{
			if (m_SelectedUnit.IsValid())
			{
				m_SelectedUnit->SetIsSelected(false);
				m_SelectedUnit = nullptr;
			}

			if (pSelectedTileData->UnitOnTile.IsValid())
			{
				pSelectedTileData->UnitOnTile->SetIsSelected(true);
				m_SelectedUnit = pSelectedTileData->UnitOnTile;
			}
		}
	}
}

void APlayerActions::_UpdateHoveredTile()
{
	FIntPoint newIndex;
	if (m_HoveredUnit.IsValid())
	{
		newIndex = m_HoveredUnit->GetIndex();
	}
	else
	{
		newIndex =  m_Grid->GetTileIndexUnderCursor(0);
	}
	
	if (m_HoveredTileIndex != newIndex)
	{
		m_Grid->RemoveStateFromTileWithNotify(m_HoveredTileIndex, ETileStateFlags::Hovered);
		
		m_HoveredTileIndex = newIndex;
		m_Grid->AddStateToTileWithNotify(m_HoveredTileIndex, ETileStateFlags::Hovered);

		_OnHoveredTileChanged();
	}
}

void APlayerActions::_UpdateHoveredUnit()
{
	AUnit* pUnderUnit= _FindUnitUnderCursor(0);
	if (m_HoveredUnit != pUnderUnit)
	{
		if (m_HoveredUnit.IsValid())
		{
			m_HoveredUnit->SetIsHovered(false);
		}

		m_HoveredUnit = pUnderUnit;
		if (m_HoveredUnit.IsValid())
		{
			m_HoveredUnit->SetIsHovered(true);
		}
	}
}

AUnit* APlayerActions::_FindUnitUnderCursor(uint8 _playerIndex)
{
	if (APlayerController* pPlayerController = UGameplayStatics::GetPlayerController(GetWorld(), _playerIndex))
	{
		FHitResult hitResult;
		bool isHit = pPlayerController->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(GTC_Unit), false, hitResult);
		if (isHit)
		{
			return Cast<AUnit>(hitResult.GetActor());
		}
		
		const FIntPoint tileIndex = m_Grid->GetTileIndexUnderCursor(_playerIndex);
		if (const FTileData* pTile = m_Grid->GetGridTileMap().Find(tileIndex))
		{
			return pTile->UnitOnTile.Get();
		}		
	}
	return nullptr;
}

void APlayerActions::_OnHoveredTileChanged()
{
	if (m_bIsLeftClickDown)
	{
		if (m_LeftClickSelectAction)
		{
			m_LeftClickSelectAction->Execute(m_HoveredTileIndex);
		}
		return;
	}

	if (m_bIsRightClickDown)
	{
		if (m_RightClickSelectAction)
		{
			m_RightClickSelectAction->Execute(m_HoveredTileIndex);
		}
		return;
	}
}

#pragma region Event Handlers
void APlayerActions::_HandleLeftClickPressed()
{
	m_bIsLeftClickDown = true;
	_UpdateHoveredUnit();
	_UpdateHoveredTile();
	if (m_LeftClickSelectAction)
	{
		m_LeftClickSelectAction->Execute(m_HoveredTileIndex);
	}

}

void APlayerActions::_HandleLeftClickReleased()
{
	m_bIsLeftClickDown = false;
}

void APlayerActions::_HandleRightClickPressed()
{
	m_bIsRightClickDown = true;
	_UpdateHoveredUnit();
	_UpdateHoveredTile();
	if (m_RightClickSelectAction)
	{
		m_RightClickSelectAction->Execute(m_HoveredTileIndex);
	}
}

void APlayerActions::_HandleRightClickReleased()
{
	m_bIsRightClickDown = false;
}

void APlayerActions::_HandleGridGenerated()
{
	if (m_Grid->IsIndexValid(m_SelectedTileIndex))
	{
		SelectTileAndUnit(m_SelectedTileIndex, true);
	}
	else
	{
		SelectTileAndUnit(FIntPoint(INVALID_POINT_VALUE), true);
	}
}

void APlayerActions::_HandleTileDataUpdated(const FIntPoint& _index)
{
	if (m_SelectedTileIndex == _index)
	{
		_HandleGridGenerated();
	}
}

void APlayerActions::_HandleUnitGridIndexChanged(AUnit* const _pUnit)
{
	if (_pUnit != m_SelectedUnit)
	{
		SelectTileAndUnit(_pUnit->GetIndex(), false);
	}
}
#pragma endregion


