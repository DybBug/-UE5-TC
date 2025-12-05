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
	CheckBox_CanUseDiagonals->OnCheckStateChanged.AddDynamic(this, &UFindPathToTargetButton::OnCanUseDiagonalsCheckBoxStateChanged);
	CheckBox_CanUseFlyingOnly->OnCheckStateChanged.AddDynamic(this, &UFindPathToTargetButton::OnCanUseFlyingOnlyCheckBoxStateChanged);
	SpinBox_Delay->OnValueChanged.AddDynamic(this, &UFindPathToTargetButton::OnDelaySpinBoxValueChanged);
	SpinBox_MaxMs->OnValueChanged.AddDynamic(this, &UFindPathToTargetButton::OnMaxMsSpinBoxValueChanged);
}

void UFindPathToTargetButton::NativePreConstruct()
{
	Super::NativePreConstruct();
	if (GetWorld() && GetWorld()->IsGameWorld())
	{		
		bool isVisible = (m_PlayerActions != nullptr) && (Cast<UFindPathToTargetAction>(m_PlayerActions->GetRightClickSelectAction()) != nullptr);
		_UpdateAllElementVisibility(isVisible);
	}
}

void UFindPathToTargetButton::OnSelectedActionsChanged(const UAbstractAction* const _leftClickAction, const UAbstractAction* const _rightClickAction)
{
	Super::OnSelectedActionsChanged(_leftClickAction, _rightClickAction);

	UFindPathToTargetAction* pFindPathToTargetAction = Cast<UFindPathToTargetAction>(m_PlayerActions->GetRightClickSelectAction());
	bool isVisible = (pFindPathToTargetAction != nullptr);
	_UpdateAllElementVisibility(isVisible);
}

void UFindPathToTargetButton::OnCanUseDiagonalsCheckBoxStateChanged(bool _bIsChecked)
{
	UFindPathToTargetAction* pFindPathToTargetAction = Cast<UFindPathToTargetAction>(m_PlayerActions->GetRightClickSelectAction());
	if (!pFindPathToTargetAction)
	{
		CheckBox_CanUseDiagonals->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	pFindPathToTargetAction->SetCanUseDiagonals(_bIsChecked);
	CheckBox_CanUseDiagonals->SetVisibility(ESlateVisibility::Visible);
}

void UFindPathToTargetButton::OnCanUseFlyingOnlyCheckBoxStateChanged(bool _bIsChecked)
{
	UFindPathToTargetAction* pFindPathToTargetAction = Cast<UFindPathToTargetAction>(m_PlayerActions->GetRightClickSelectAction());
	if (!pFindPathToTargetAction)
	{
		CheckBox_CanUseFlyingOnly->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	pFindPathToTargetAction->SetCanUseFlyingOnly(_bIsChecked);
	CheckBox_CanUseFlyingOnly->SetVisibility(ESlateVisibility::Visible);
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

void UFindPathToTargetButton::_UpdateAllElementVisibility(bool _isVisible)
{
	if (_isVisible)
	{
		CheckBox_CanUseDiagonals->SetVisibility(ESlateVisibility::Visible);
		CheckBox_CanUseFlyingOnly->SetVisibility(ESlateVisibility::Visible);
		SpinBox_Delay->SetVisibility(ESlateVisibility::Visible);
		SpinBox_MaxMs->SetVisibility(ESlateVisibility::Visible);	
	}
	else
	{
		CheckBox_CanUseDiagonals->SetVisibility(ESlateVisibility::Collapsed);
		CheckBox_CanUseFlyingOnly->SetVisibility(ESlateVisibility::Collapsed);
		SpinBox_Delay->SetVisibility(ESlateVisibility::Collapsed);
		SpinBox_MaxMs->SetVisibility(ESlateVisibility::Collapsed);
	}
}


