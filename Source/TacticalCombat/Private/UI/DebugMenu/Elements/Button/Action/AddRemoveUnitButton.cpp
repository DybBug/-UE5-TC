// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DebugMenu/Elements/Button/Action/AddRemoveUnitButton.h"

#include "Action/AbstractAction.h"
#include "Action/Combat/AddRemoveUnitAction.h"
#include "Components/Button.h"
#include "Core/PlayerActions.h"
#include "UI/DebugMenu/Elements/UnitButtonList.h"

void UAddRemoveUnitButton::NativeConstruct()
{
	Super::NativeConstruct();
	_UpdateWidget(Button_UnitList->GetSelectedUnitType());
	Button_UnitList->OnSelectedUnitButtonChanged.AddDynamic(this, &UAddRemoveUnitButton::_OnSelectedUnitButtonChanged);
}

void UAddRemoveUnitButton::OnSelectedActionsChanged(const UAbstractAction* const _leftClickAction, const UAbstractAction* const rightClickAction)
{
	Super::OnSelectedActionsChanged(_leftClickAction, rightClickAction);
	_UpdateWidget(Button_UnitList->GetSelectedUnitType());
}

void UAddRemoveUnitButton::_UpdateWidget(ETacticalUnitType _unitType)
{
	Button_UnitList->SetVisibility(ESlateVisibility::Collapsed);

	// 좌클릭
	{
		UAbstractAction* pLeftAction = m_PlayerActions->GetLeftClickSelectAction();
		UAddRemoveUnitAction* pAddRemoveUnitAction = Cast<UAddRemoveUnitAction>(pLeftAction);
		if (pAddRemoveUnitAction)
		{
			pAddRemoveUnitAction->SetIsAddingUnit(true);
			pAddRemoveUnitAction->SetUnitType(_unitType);
			Button_UnitList->SetVisibility(ESlateVisibility::Visible);

		}
	}

	// 우클릭
	{		
		UAbstractAction* pRightAction = m_PlayerActions->GetRightClickSelectAction();
		UAddRemoveUnitAction* pAddRemoveUnitAction = Cast<UAddRemoveUnitAction>(pRightAction);
		if (pAddRemoveUnitAction)
		{
			pAddRemoveUnitAction->SetIsAddingUnit(false);
			Button_UnitList->SetVisibility(ESlateVisibility::Visible);
		}
	}
	
}


void UAddRemoveUnitButton::_OnSelectedUnitButtonChanged(ETacticalUnitType _unitType)
{
	_UpdateWidget(_unitType);
}
