// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DebugMenu/Elements/Button/Action/SetTileTypeActionButton.h"
#include "Core/PlayerActions.h"
#include "Action/Grid/SetTileTypeAction.h"
#include "Grid/Grid.h"
#include "Components/ComboBoxString.h"


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

	uint8 flag = 0;
	while (flag < static_cast<int32>(ETileType::Max))
	{
		ComboBoxString->AddOption(StaticEnum<ETileType>()->GetNameStringByValue(flag));
		flag = (flag == 0) ? 1 : flag << 1;
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
	if (IsCurrentSelectedAction())
	{		
		ComboBoxString->SetVisibility(ESlateVisibility::Collapsed);
		if (USetTileTypeAction* pSetTileTypeAction = Cast<USetTileTypeAction>(m_PlayerActions->GetLeftClickSelectAction()))
		{
			int32 selectedIndex = ComboBoxString->GetSelectedIndex();
			ETileType type = (ETileType)(selectedIndex == 0 ? 0 : (1 << (selectedIndex - 1)));
			pSetTileTypeAction->SetTileType(type);
			ComboBoxString->SetVisibility(ESlateVisibility::Visible);
		}
			
		if (USetTileTypeAction* pSetTileTypeAction = Cast<USetTileTypeAction>(m_PlayerActions->GetRightClickSelectAction()))
		{
			pSetTileTypeAction->SetTileType(ETileType::Normal);
			ComboBoxString->SetVisibility(ESlateVisibility::Visible);
		}
	}
}