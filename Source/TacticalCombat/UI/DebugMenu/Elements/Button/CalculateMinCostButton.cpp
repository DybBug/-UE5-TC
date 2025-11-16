// Fill out your copyright notice in the Description page of Project Settings.


#include "CalculateMinCostButton.h"

#include "Components/CheckBox.h"
#include "TacticalCombat/Core/Actions/Pathfinding/SelectTileWithCalculateMinCost.h"
#include "TacticalCombat/Core/Actions/Pathfinding/ShowTileNeighborsAction.h"

void UCalculateMinCostButton::NativeConstruct()
{
	Super::NativeConstruct();
	CheckBox_UseDiagonals->OnCheckStateChanged.AddDynamic(this, &UCalculateMinCostButton::OnUseDiagonalsCheckBoxStateChanged);
}

void UCalculateMinCostButton::NativePreConstruct()
{
	Super::NativePreConstruct();
	CheckBox_UseDiagonals->SetVisibility(ESlateVisibility::Collapsed);
}

void UCalculateMinCostButton::OnSelectedActionsChanged(const UAbstractAction* const _leftClickAction, const UAbstractAction* const _rightClickAction)
{
	Super::OnSelectedActionsChanged(_leftClickAction, _rightClickAction);

	USelectTileWithCalculateMinCost* pSelectTileWithCalculateMinCost = Cast<USelectTileWithCalculateMinCost>(m_PlayerActions->GetLeftClickSelectAction());
	if (!pSelectTileWithCalculateMinCost)
	{
		CheckBox_UseDiagonals->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}
	
	CheckBox_UseDiagonals->SetVisibility(ESlateVisibility::Visible);
}

void UCalculateMinCostButton::OnUseDiagonalsCheckBoxStateChanged(bool _bIsChecked)
{
	USelectTileWithCalculateMinCost* pSelectTileWithCalculateMinCost = Cast<USelectTileWithCalculateMinCost>(m_PlayerActions->GetLeftClickSelectAction());
	if (!pSelectTileWithCalculateMinCost)
	{
		CheckBox_UseDiagonals->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	pSelectTileWithCalculateMinCost->SetUseDiagonals(_bIsChecked);
	CheckBox_UseDiagonals->SetVisibility(ESlateVisibility::Visible);
}