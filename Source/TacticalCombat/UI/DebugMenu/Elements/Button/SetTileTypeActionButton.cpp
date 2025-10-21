// Fill out your copyright notice in the Description page of Project Settings.


#include "SetTileTypeActionButton.h"
#include "Components/ComboBoxString.h"
#include "Kismet/GameplayStatics.h"
#include "TacticalCombat/Core/PlayerActions.h"
#include "TacticalCombat/Core/Actions/SetTileTypeAction.h"
#include "TacticalCombat/Core/Grid/Grid.h"


void USetTileTypeActionButton::NativeConstruct()
{
	Super::NativeConstruct();

	ComboBoxString->OnSelectionChanged.AddDynamic(this, &USetTileTypeActionButton::_OnComboBoxTileTypeSelectionChanged);
	_SetTileType();
}

void USetTileTypeActionButton::NativePreConstruct()
{
	Super::NativePreConstruct();

	ComboBoxString->ClearOptions();
	
	for (ETileType tileType : TEnumRange<ETileType>())
	{
		ComboBoxString->AddOption(StaticEnum<ETileType>()->GetNameStringByValue((uint8)tileType));
	}
	ComboBoxString->SetSelectedIndex(0);	
}

void USetTileTypeActionButton::OnSelectedActionsChanged(const UAbstractAction* const _leftClickAction, const UAbstractAction* const _rightClickAction)
{
	Super::OnSelectedActionsChanged(_leftClickAction, _rightClickAction);
	_SetTileType();
}

void USetTileTypeActionButton::_OnComboBoxTileTypeSelectionChanged(FString _selectedItem, ESelectInfo::Type _selectionType)
{
	if (m_PlayerActions.IsValid())
	{
		_SetTileType();
	}
}

void USetTileTypeActionButton::_SetTileType()
{
	ComboBoxString->SetVisibility(ESlateVisibility::Collapsed);
	if (USetTileTypeAction* pSetTileTypeAction = Cast<USetTileTypeAction>(m_PlayerActions->GetLeftClickSelectAction()))
	{
		int32 selectedIndex = ComboBoxString->GetSelectedIndex();
		pSetTileTypeAction->SetTileType((ETileType)selectedIndex);
		ComboBoxString->SetVisibility(ESlateVisibility::Visible);
	}
		
	if (USetTileTypeAction* pSetTileTypeAction = Cast<USetTileTypeAction>(m_PlayerActions->GetRightClickSelectAction()))
	{
		pSetTileTypeAction->SetTileType(ETileType::Normal);
		ComboBoxString->SetVisibility(ESlateVisibility::Visible);
	}
}
