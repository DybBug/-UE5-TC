// Fill out your copyright notice in the Description page of Project Settings.


#include "FindPathToTargetButton.h"
#include "Components/CheckBox.h"
#include "TacticalCombat/Core/PlayerActions.h"
#include "TacticalCombat/Core/Actions/Pathfinding/FindPathToTargetAction.h"
#include "TacticalCombat/Core/Actions/Pathfinding/ShowTileNeighborsAction.h"
#include "TacticalCombat/UI/DebugMenu/Elements/SpinBox/WithNameSpinBox.h"

void UFindPathToTargetButton::NativeConstruct()
{
	Super::NativeConstruct();
	CheckBox_UseDiagonals->OnCheckStateChanged.AddDynamic(this, &UFindPathToTargetButton::OnUseDiagonalsCheckBoxStateChanged);
	SpinBox_Delay->OnValueChanged.AddDynamic(this, &UFindPathToTargetButton::OnDelaySpinBoxValueChanged);
	SpinBox_MaxMs->OnValueChanged.AddDynamic(this, &UFindPathToTargetButton::OnMaxMsSpinBoxValueChanged);
}

void UFindPathToTargetButton::NativePreConstruct()
{
	Super::NativePreConstruct();

	bool isVisible = (m_PlayerActions != nullptr) && (Cast<UFindPathToTargetAction>(m_PlayerActions->GetRightClickSelectAction()) != nullptr);
	if (isVisible)
	{
		CheckBox_UseDiagonals->SetVisibility(ESlateVisibility::Visible);
		SpinBox_Delay->SetVisibility(ESlateVisibility::Visible);
		SpinBox_MaxMs->SetVisibility(ESlateVisibility::Visible);	
	}
	else
	{
		CheckBox_UseDiagonals->SetVisibility(ESlateVisibility::Collapsed);
		SpinBox_Delay->SetVisibility(ESlateVisibility::Collapsed);
		SpinBox_MaxMs->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UFindPathToTargetButton::OnSelectedActionsChanged(const UAbstractAction* const _leftClickAction, const UAbstractAction* const _rightClickAction)
{
	Super::OnSelectedActionsChanged(_leftClickAction, _rightClickAction);

	UFindPathToTargetAction* pFindPathToTargetAction = Cast<UFindPathToTargetAction>(m_PlayerActions->GetRightClickSelectAction());
	if (!pFindPathToTargetAction)
	{
		CheckBox_UseDiagonals->SetVisibility(ESlateVisibility::Collapsed);
		SpinBox_Delay->SetVisibility(ESlateVisibility::Collapsed);
		SpinBox_MaxMs->SetVisibility(ESlateVisibility::Collapsed);

		return;
	}
	
	CheckBox_UseDiagonals->SetVisibility(ESlateVisibility::Visible);
	SpinBox_Delay->SetVisibility(ESlateVisibility::Visible);
	SpinBox_MaxMs->SetVisibility(ESlateVisibility::Visible);

}

void UFindPathToTargetButton::OnUseDiagonalsCheckBoxStateChanged(bool _bIsChecked)
{
	UFindPathToTargetAction* pFindPathToTargetAction = Cast<UFindPathToTargetAction>(m_PlayerActions->GetRightClickSelectAction());
	if (!pFindPathToTargetAction)
	{
		CheckBox_UseDiagonals->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	pFindPathToTargetAction->SetUseDiagonals(_bIsChecked);
	CheckBox_UseDiagonals->SetVisibility(ESlateVisibility::Visible);
}


void UFindPathToTargetButton::OnDelaySpinBoxValueChanged(float _value)
{
	UFindPathToTargetAction* pFindPathToTargetAction = Cast<UFindPathToTargetAction>(m_PlayerActions->GetRightClickSelectAction());
	if (!pFindPathToTargetAction)
	{
		SpinBox_Delay->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	pFindPathToTargetAction->SetDelayBetweenIterations(_value);
	SpinBox_Delay->SetVisibility(ESlateVisibility::Visible);
}

void UFindPathToTargetButton::OnMaxMsSpinBoxValueChanged(float _value)
{
	UFindPathToTargetAction* pFindPathToTargetAction = Cast<UFindPathToTargetAction>(m_PlayerActions->GetRightClickSelectAction());
	if (!pFindPathToTargetAction)
	{
		SpinBox_MaxMs->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	pFindPathToTargetAction->SetMaxMs(_value);
	SpinBox_MaxMs->SetVisibility(ESlateVisibility::Visible);
}

