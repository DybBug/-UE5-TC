// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DebugMenu/Elements/Button/Action/FindPathToTargetButton.h"
#include "Core/PlayerActions.h"
#include "Action/Pathfinding/FindPathToTargetAction.h"
#include "Action/Pathfinding/SelectAndGenReachablesAction.h"
#include "Action/Pathfinding/ShowTileNeighborsAction.h"
#include "UI/DebugMenu/Elements/SpinBox/WithNameSpinBox.h"
#include "Components/CheckBox.h"

void UFindPathToTargetButton::NativeConstruct()
{
	Super::NativeConstruct();
	CheckBox_CanUseDiagonals->OnCheckStateChanged.AddDynamic(this, &UFindPathToTargetButton::HandleCanUseDiagonalsCheckBoxStateChanged);
	CheckBox_CanUseFlyingOnly->OnCheckStateChanged.AddDynamic(this, &UFindPathToTargetButton::HandleCanUseFlyingOnlyCheckBoxStateChanged);
	SpinBox_Length->OnValueChanged.AddDynamic(this, &UFindPathToTargetButton::HandleLengthSpinBoxValueChange);
	SpinBox_Delay->OnValueChanged.AddDynamic(this, &UFindPathToTargetButton::HandleDelaySpinBoxValueChanged);
	SpinBox_MaxMs->OnValueChanged.AddDynamic(this, &UFindPathToTargetButton::HandleMaxMsSpinBoxValueChanged);
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
	
	_UpdateAllElementVisibility(IsCurrentSelectedAction());
}

void UFindPathToTargetButton::HandleCanUseDiagonalsCheckBoxStateChanged(bool _bIsChecked)
{
	if (IsCurrentSelectedAction())
	{
		CheckBox_CanUseDiagonals->SetVisibility(ESlateVisibility::Visible);
		
		if (USelectAndGenReachablesAction* pSelectAndGenReachableAction = Cast<USelectAndGenReachablesAction>(m_PlayerActions->GetLeftClickSelectAction()))
		{
			pSelectAndGenReachableAction->SetCanUseDiagonals(_bIsChecked);
		}
		
		if (UFindPathToTargetAction* pFindPathToTargetAction = Cast<UFindPathToTargetAction>(m_PlayerActions->GetRightClickSelectAction()))
		{		
			pFindPathToTargetAction->SetCanUseDiagonals(_bIsChecked);
		}
	}
	else
	{
		CheckBox_CanUseDiagonals->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UFindPathToTargetButton::HandleCanUseFlyingOnlyCheckBoxStateChanged(bool _bIsChecked)
{
	if (IsCurrentSelectedAction())
	{
		CheckBox_CanUseFlyingOnly->SetVisibility(ESlateVisibility::Visible);
		
		if (USelectAndGenReachablesAction* pSelectAndGenReachableAction = Cast<USelectAndGenReachablesAction>(m_PlayerActions->GetLeftClickSelectAction()))
		{
			pSelectAndGenReachableAction->SetCanUseFlyingOnly(_bIsChecked);
		}
		
		if (UFindPathToTargetAction* pFindPathToTargetAction = Cast<UFindPathToTargetAction>(m_PlayerActions->GetRightClickSelectAction()))
		{
			pFindPathToTargetAction->SetCanUseFlyingOnly(_bIsChecked);
		}
	}
	else
	{
		CheckBox_CanUseFlyingOnly->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UFindPathToTargetButton::HandleLengthSpinBoxValueChange(float _value)
{
	if (IsCurrentSelectedAction())
	{
		SpinBox_Length->SetVisibility(ESlateVisibility::Visible);
		
		if (USelectAndGenReachablesAction* pSelectAndGenReachableAction = Cast<USelectAndGenReachablesAction>(m_PlayerActions->GetLeftClickSelectAction()))
		{
			pSelectAndGenReachableAction->SetMaxPathLength(FMath::TruncToInt32(_value));
		}
		
		if (UFindPathToTargetAction* pFindPathToTargetAction = Cast<UFindPathToTargetAction>(m_PlayerActions->GetRightClickSelectAction()))
		{
			pFindPathToTargetAction->SetMaxPathLength(FMath::TruncToInt32(_value));
		}
	}
	else
	{
		SpinBox_Length->SetVisibility(ESlateVisibility::Collapsed);
	}
}


void UFindPathToTargetButton::HandleDelaySpinBoxValueChanged(float _value)
{
	if (IsCurrentSelectedAction())
	{
		SpinBox_Delay->SetVisibility(ESlateVisibility::Visible);

		if (USelectAndGenReachablesAction* pSelectAndGenReachableAction = Cast<USelectAndGenReachablesAction>(m_PlayerActions->GetLeftClickSelectAction()))
		{
			pSelectAndGenReachableAction->SetDelayBetweenIterations(_value);
		}

		if (UFindPathToTargetAction* pFindPathToTargetAction = Cast<UFindPathToTargetAction>(m_PlayerActions->GetRightClickSelectAction()))
		{
			pFindPathToTargetAction->SetDelayBetweenIterations(_value);	
		}
	}
	else
	{
		SpinBox_Delay->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UFindPathToTargetButton::HandleMaxMsSpinBoxValueChanged(float _value)
{
	if (IsCurrentSelectedAction())
	{
		SpinBox_MaxMs->SetVisibility(ESlateVisibility::Visible);

		if (USelectAndGenReachablesAction* pSelectAndGenReachableAction = Cast<USelectAndGenReachablesAction>(m_PlayerActions->GetLeftClickSelectAction()))
		{
			pSelectAndGenReachableAction->SetMaxMs(_value);
		}

		if (UFindPathToTargetAction* pFindPathToTargetAction = Cast<UFindPathToTargetAction>(m_PlayerActions->GetRightClickSelectAction()))
		{
			pFindPathToTargetAction->SetMaxMs(_value);
		}
	}
	else
	{
		SpinBox_MaxMs->SetVisibility(ESlateVisibility::Collapsed);
	}

}

void UFindPathToTargetButton::_UpdateAllElementVisibility(bool _isVisible)
{
	if (_isVisible)
	{
		CheckBox_CanUseDiagonals->SetVisibility(ESlateVisibility::Visible);
		CheckBox_CanUseFlyingOnly->SetVisibility(ESlateVisibility::Visible);
		SpinBox_Length->SetVisibility(ESlateVisibility::Visible);
		SpinBox_Delay->SetVisibility(ESlateVisibility::Visible);
		SpinBox_MaxMs->SetVisibility(ESlateVisibility::Visible);	
	}
	else
	{
		CheckBox_CanUseDiagonals->SetVisibility(ESlateVisibility::Collapsed);
		CheckBox_CanUseFlyingOnly->SetVisibility(ESlateVisibility::Collapsed);
		SpinBox_Length->SetVisibility(ESlateVisibility::Collapsed);
		SpinBox_Delay->SetVisibility(ESlateVisibility::Collapsed);
		SpinBox_MaxMs->SetVisibility(ESlateVisibility::Collapsed);
	}
}


