// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DebugMenu/Elements/Button/Action/CalculateMinCostButton.h"

#include "Core/PlayerActions.h"
#include "Action/Pathfinding/SelectTileWithCalculateMinCost.h"
#include "Action/Pathfinding/ShowTileNeighborsAction.h"
#include "Components/CheckBox.h"

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

	if (IsCurrentSelectedAction())
	{
		CheckBox_UseDiagonals->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		CheckBox_UseDiagonals->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UCalculateMinCostButton::OnUseDiagonalsCheckBoxStateChanged(bool _bIsChecked)
{
	if (IsCurrentSelectedAction())
	{
		USelectTileWithCalculateMinCost* pSelectTileWithCalculateMinCost = Cast<USelectTileWithCalculateMinCost>(m_PlayerActions->GetLeftClickSelectAction());
		pSelectTileWithCalculateMinCost->SetUseDiagonals(_bIsChecked);
		CheckBox_UseDiagonals->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		CheckBox_UseDiagonals->SetVisibility(ESlateVisibility::Collapsed);
	}
}