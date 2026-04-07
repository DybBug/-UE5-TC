// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DebugMenu/Elements/Button/Action/ShowSpellRangePatternButton.h"

#include "Action/Grid/SetTileTypeAction.h"
#include "Action/Grid/ShowSpellRangePatternAction.h"
#include "Combat/Spells/Types/SpellCast.h"
#include "Components/ComboBoxString.h"
#include "Core/PlayerActions.h"
#include "Grid/Grid.h"
#include "Library/SpellLibrary.h"
#include "UI/DebugMenu/Elements/SpinBox/Vector2DWithNameSpinBox.h"

void UShowSpellRangePatternButton::NativePreConstruct()
{
	Super::NativePreConstruct();

	ComboBoxString->ClearOptions();
	
	for (ESpellRangePattern spellRangePattern : TEnumRange<ESpellRangePattern>())
	{
		FString spellRangePatternAsStr = StaticEnum<ESpellRangePattern>()->GetNameStringByValue(static_cast<uint8>(spellRangePattern));
		ComboBoxString->AddOption(spellRangePatternAsStr);
	}
	ComboBoxString->SetSelectedIndex(0);	
}

void UShowSpellRangePatternButton::NativeConstruct()
{
	Super::NativeConstruct();

	ComboBoxString->OnSelectionChanged.AddDynamic(this, &UShowSpellRangePatternButton::_HandleSpellRangePatternComboBoxSelectionChanged);
	SpinBox_Range->OnValueChanged.AddDynamic(this, &UShowSpellRangePatternButton::_HandleRangeChanged);
	_SetSpellRangePattern();
}

void UShowSpellRangePatternButton::OnSelectedActionsChanged(const UAbstractAction* const _leftClickAction, const UAbstractAction* const _rightClickAction)
{
	Super::OnSelectedActionsChanged(_leftClickAction, _rightClickAction);
	_SetSpellRangePattern();
}

void UShowSpellRangePatternButton::_HandleSpellRangePatternComboBoxSelectionChanged(FString _selectedItem, ESelectInfo::Type _selectionType)
{
	if (!m_PlayerActions.IsValid()) return;
	
	_SetSpellRangePattern();
}

void UShowSpellRangePatternButton::_HandleRangeChanged(const FVector2D& _value)
{
	if (!m_PlayerActions.IsValid()) return;

	_SetSpellRangePattern();
}

void UShowSpellRangePatternButton::_SetSpellRangePattern()
{
	ComboBoxString->SetVisibility(ESlateVisibility::Collapsed);
	if (IsCurrentSelectedAction())
	{		
		if (UShowSpellRangePatternAction* pShowSpellRangePatternAction = Cast<UShowSpellRangePatternAction>(m_PlayerActions->GetLeftClickSelectAction()))
		{
			int selectedPatternIndex  = ComboBoxString->GetSelectedIndex();
			ESpellRangePattern spellRangePattern = static_cast<ESpellRangePattern>(StaticEnum<ESpellRangePattern>()->GetValueByIndex(selectedPatternIndex));
			const FVector2D& spellRange = SpinBox_Range->GetValue();
			pShowSpellRangePatternAction->SetCast(
				{
					.RangePattern = spellRangePattern,
					.RangeBounds = FIntPoint(spellRange.X, spellRange.Y)
				}
			);
			ComboBoxString->SetVisibility(ESlateVisibility::Visible);
			SpinBox_Range->SetVisibility(ESlateVisibility::Visible);

			GetWorld()->GetTimerManager().SetTimer(m_hDelayTimer, [pShowSpellRangePatternAction]() {
				pShowSpellRangePatternAction->ShowSpellRangePattern();
				}, 0.1f, false);
		}		
	}
}
