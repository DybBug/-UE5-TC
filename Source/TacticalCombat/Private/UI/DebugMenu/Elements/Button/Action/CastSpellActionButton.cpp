// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DebugMenu/Elements/Button/Action/CastSpellActionButton.h"

#include "Core/PlayerActions.h"
#include "Action/Combat/CastSpellAction.h"
#include "Action/Grid/ShowSpellRangePatternAction.h"
#include "Components/ComboBoxString.h"
#include "Library/SpellLibrary.h"

void UCastSpellActionButton::NativePreConstruct()
{
	Super::NativePreConstruct();

	ComboBoxString->ClearOptions();
	for (const ESpellType& spellType : TEnumRange<ESpellType>())
	{
		FString spellTypeName = StaticEnum<ESpellType>()->GetNameStringByValue(static_cast<uint8>(spellType));
		ComboBoxString->AddOption(spellTypeName);
	}
	ComboBoxString->SetSelectedIndex(0);
}

void UCastSpellActionButton::NativeConstruct()
{
	Super::NativeConstruct();
	ComboBoxString->OnSelectionChanged.AddDynamic(this, &UCastSpellActionButton::_OnComboBoxSpellTypeSelectionChanged);
	_ApplySelectedSpellType();
}

void UCastSpellActionButton::OnSelectedActionsChanged(const UAbstractAction* const _leftClickAction,
	const UAbstractAction* const _rightClickAction)
{
	Super::OnSelectedActionsChanged(_leftClickAction, _rightClickAction);
	_ApplySelectedSpellType();
}

void UCastSpellActionButton::_OnComboBoxSpellTypeSelectionChanged(FString _selectedItem,
	ESelectInfo::Type _selectionType)
{
	if (m_PlayerActions.IsValid())
	{
		_ApplySelectedSpellType();
	}
}

void UCastSpellActionButton::_ApplySelectedSpellType()
{
	ComboBoxString->SetVisibility(ESlateVisibility::Collapsed);
	if (IsCurrentSelectedAction())
	{		
		ComboBoxString->SetVisibility(ESlateVisibility::Visible);
		// Left Click Action
		if (UShowSpellRangePatternAction* pShowSpellRangePatternAction = Cast<UShowSpellRangePatternAction>(m_PlayerActions->GetLeftClickSelectAction()))
		{
			ESpellType castingSpellType = static_cast<ESpellType>(ComboBoxString->GetSelectedIndex());
			FSpellTableRow spellTableRow = USpellLibrary::GetSpellRowDataFromType(castingSpellType);
			pShowSpellRangePatternAction->SetCast(spellTableRow.SpellCast);

			int selectedPatternIndex = ComboBoxString->GetSelectedIndex();
			ESpellRangePattern spellRangePattern = static_cast<ESpellRangePattern>(StaticEnum<ESpellRangePattern>()->GetValueByIndex(selectedPatternIndex));
			GetWorld()->GetTimerManager().SetTimer(m_hDelayTimer, [pShowSpellRangePatternAction]() {
				pShowSpellRangePatternAction->ShowSpellRangePattern();
				}, 0.1f, false);
		}

			
		// Right Click Action
		if (UCastSpellAction* pCastSpellAction = Cast<UCastSpellAction>(m_PlayerActions->GetRightClickSelectAction()))
		{
			int32 selectedIndex = ComboBoxString->GetSelectedIndex();
			pCastSpellAction->SetSpellType((ESpellType)(selectedIndex));

		}
	}
}