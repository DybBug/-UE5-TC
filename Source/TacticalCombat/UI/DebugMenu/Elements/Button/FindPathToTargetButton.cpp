// Fill out your copyright notice in the Description page of Project Settings.


#include "FindPathToTargetButton.h"
#include "Components/CheckBox.h"
#include "TacticalCombat/Core/PlayerActions.h"
#include "TacticalCombat/Core/Actions/Pathfinding/FindPathToTargetAction.h"
#include "TacticalCombat/Core/Actions/Pathfinding/ShowTileNeighborsAction.h"

void UFindPathToTargetButton::NativeConstruct()
{
	Super::NativeConstruct();
	CheckBox_UseDiagonals->OnCheckStateChanged.AddDynamic(this, &UFindPathToTargetButton::OnUseDiagonalsCheckBoxStateChanged);
}

void UFindPathToTargetButton::NativePreConstruct()
{
	Super::NativePreConstruct();
	CheckBox_UseDiagonals->SetVisibility(ESlateVisibility::Collapsed);
}

void UFindPathToTargetButton::OnSelectedActionsChanged(const UAbstractAction* const _leftClickAction, const UAbstractAction* const _rightClickAction)
{
	Super::OnSelectedActionsChanged(_leftClickAction, _rightClickAction);

	UFindPathToTargetAction* pFindpathToTargetAction = Cast<UFindPathToTargetAction>(m_PlayerActions->GetLeftClickSelectAction());
	if (!pFindpathToTargetAction)
	{
		CheckBox_UseDiagonals->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}
	
	CheckBox_UseDiagonals->SetVisibility(ESlateVisibility::Visible);
}

void UFindPathToTargetButton::OnUseDiagonalsCheckBoxStateChanged(bool _bIsChecked)
{
	UShowTileNeighborsAction* pShowTileNeighborsAction = Cast<UShowTileNeighborsAction>(m_PlayerActions->GetLeftClickSelectAction());
	if (!pShowTileNeighborsAction)
	{
		CheckBox_UseDiagonals->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	pShowTileNeighborsAction->SetUseDiagonals(_bIsChecked);
	CheckBox_UseDiagonals->SetVisibility(ESlateVisibility::Visible);
}
