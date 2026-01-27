// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DebugMenu/Elements/Button/Action/ShowTileNeighborsButton.h"
#include "Core/PlayerActions.h"
#include "Action/Pathfinding/ShowTileNeighborsAction.h"
#include "Components/CheckBox.h"

void UShowTileNeighborsButton::NativeConstruct()
{
	Super::NativeConstruct();
	CheckBox_UseDiagonals->OnCheckStateChanged.AddDynamic(this, &UShowTileNeighborsButton::OnUseDiagonalsCheckBoxStateChanged);
}

void UShowTileNeighborsButton::NativePreConstruct()
{
	Super::NativePreConstruct();
	CheckBox_UseDiagonals->SetVisibility(ESlateVisibility::Collapsed);
}

void UShowTileNeighborsButton::OnSelectedActionsChanged(const UAbstractAction* const _leftClickAction, const UAbstractAction* const _rightClickAction)
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

void UShowTileNeighborsButton::OnUseDiagonalsCheckBoxStateChanged(bool _bIsChecked)
{
	if (IsCurrentSelectedAction())
	{
		UShowTileNeighborsAction* pShowTileNeighborsAction = Cast<UShowTileNeighborsAction>(m_PlayerActions->GetLeftClickSelectAction());
		pShowTileNeighborsAction->SetUseDiagonals(_bIsChecked);
		CheckBox_UseDiagonals->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		CheckBox_UseDiagonals->SetVisibility(ESlateVisibility::Collapsed);
	}
}
