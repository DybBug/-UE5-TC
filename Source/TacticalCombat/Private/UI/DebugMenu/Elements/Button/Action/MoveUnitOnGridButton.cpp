// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DebugMenu/Elements/Button/Action/MoveUnitOnGridButton.h"

#include "Action/Pathfinding/MoveUnitOnGridAction.h"
#include "Core/PlayerActions.h"
#include "UI/DebugMenu/Elements/SpinBox/WithNameSpinBox.h"

void UMoveUnitOnGridButton::NativeConstruct()
{
	Super::NativeConstruct();

	SpinBox_MoveDurationPerTile->OnValueChanged.AddDynamic(this, &UMoveUnitOnGridButton::_HandleMaxMsValueChaged);
}

void UMoveUnitOnGridButton::OnSelectedActionsChanged(const UAbstractAction* const _leftClickAction,
	const UAbstractAction* const _rightClickAction)
{
	Super::OnSelectedActionsChanged(_leftClickAction, _rightClickAction);
	if (UMoveUnitOnGridAction* pMoveUnitOnGridAction = Cast<UMoveUnitOnGridAction>( m_PlayerActions->GetRightClickSelectAction()))
	{
		pMoveUnitOnGridAction->SetMoveDurationPerTile(SpinBox_MoveDurationPerTile->GetValue());
	}
	_UpdateSpinBoxMaxMsVisual();
}

void UMoveUnitOnGridButton::_UpdateSpinBoxMaxMsVisual()
{
	if (!m_PlayerActions.IsValid()) return;
	
	if (IsCurrentSelectedAction())
	{
		SpinBox_MoveDurationPerTile->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		SpinBox_MoveDurationPerTile->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UMoveUnitOnGridButton::_HandleMaxMsValueChaged(float _value)
{
	if (!m_PlayerActions.IsValid()) return;

	if (UMoveUnitOnGridAction* pMoveUnitOnGridAction = Cast<UMoveUnitOnGridAction>( m_PlayerActions->GetRightClickSelectAction()))
	{
		pMoveUnitOnGridAction->SetMoveDurationPerTile(_value);
	}
	
	_UpdateSpinBoxMaxMsVisual();
}
