// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DebugMenu/Elements/Button/Action/ShowSpellRangePatternButton.h"

#include "Action/Grid/SetTileTypeAction.h"
#include "Action/Grid/ShowSpellRangePatternAction.h"
#include "Combat/Spells/Types/SpellCast.h"
#include "Components/ComboBoxString.h"
#include "Core/PlayerActions.h"
#include "Grid/Grid.h"
#include "Library/SpellLibrary.h"
#include "UI/DebugMenu/Elements/SpinBox/WithNameSpinBox.h"
#include "UI/DebugMenu/Elements/SpinBox/Vector2DWithNameSpinBox.h"
#include "Components/CheckBox.h"

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
	CheckBox_LineOfSight->OnCheckStateChanged.AddDynamic(this, &UShowSpellRangePatternButton::_HandleLineOfSightCheckBoxComponentStateChanged);
	SpinBox_HeightFromGround->OnValueChanged.AddDynamic(this, &UShowSpellRangePatternButton::_HandleHeightFromGroundChanged);
	SpinBox_OffsetFromCenter->OnValueChanged.AddDynamic(this, &UShowSpellRangePatternButton::_HandleOffsetFromCenterChanged);
	CheckBox_DrawDebugLine->OnCheckStateChanged.AddDynamic(this, &UShowSpellRangePatternButton::_HandleDrawDebugLineChanged);
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

void UShowSpellRangePatternButton::_HandleLineOfSightCheckBoxComponentStateChanged(bool _bIsChecked)
{
	if (!m_PlayerActions.IsValid()) return;
	_SetSpellRangePattern();
}

void UShowSpellRangePatternButton::_HandleHeightFromGroundChanged(float _value)
{
	if (!m_PlayerActions.IsValid()) return;
	_SetSpellRangePattern();

}

void UShowSpellRangePatternButton::_HandleOffsetFromCenterChanged(float _value)
{
	if (!m_PlayerActions.IsValid()) return;
	_SetSpellRangePattern();

}

void UShowSpellRangePatternButton::_HandleDrawDebugLineChanged(bool _bIsChecked)
{
	if (!m_PlayerActions.IsValid()) return;
	_SetSpellRangePattern();

}

void UShowSpellRangePatternButton::_SetSpellRangePattern()
{
	ComboBoxString->SetVisibility(ESlateVisibility::Collapsed);
	SpinBox_Range->SetVisibility(ESlateVisibility::Collapsed);
	CheckBox_LineOfSight->SetVisibility(ESlateVisibility::Collapsed);
	SpinBox_HeightFromGround->SetVisibility(ESlateVisibility::Collapsed);
	SpinBox_OffsetFromCenter->SetVisibility(ESlateVisibility::Collapsed);
	CheckBox_DrawDebugLine->SetVisibility(ESlateVisibility::Collapsed);

	if (IsCurrentSelectedAction())
	{		
		if (UShowSpellRangePatternAction* pShowSpellRangePatternAction = Cast<UShowSpellRangePatternAction>(m_PlayerActions->GetLeftClickSelectAction()))
		{
			int selectedPatternIndex  = ComboBoxString->GetSelectedIndex();
			ESpellRangePattern spellRangePattern = static_cast<ESpellRangePattern>(StaticEnum<ESpellRangePattern>()->GetValueByIndex(selectedPatternIndex));
			const FVector2D& spellRange = SpinBox_Range->GetValue();
			bool bIsRequireLineOfSight = CheckBox_LineOfSight->IsChecked();
			float heightFromGround = SpinBox_HeightFromGround->GetValue();
			float offsetFromCenter = SpinBox_OffsetFromCenter->GetValue();
			bool bIsDrawDebugLine = CheckBox_DrawDebugLine->IsChecked();

			pShowSpellRangePatternAction->SetCast(
				{
					.RangePattern = spellRangePattern,
					.RangeBounds = FIntPoint(spellRange.X, spellRange.Y),
					.bIsRequireLineOfSight = bIsRequireLineOfSight,
					.LineOfSight = {
										.HeightFromGround = heightFromGround,
										.OffsetFromCenter = offsetFromCenter,
										.bIsDrawDebugLine = bIsDrawDebugLine 
									}
				}
			);

			ComboBoxString->SetVisibility(ESlateVisibility::Visible);
			SpinBox_Range->SetVisibility(ESlateVisibility::Visible);
			CheckBox_LineOfSight->SetVisibility(ESlateVisibility::Visible);
			SpinBox_HeightFromGround->SetVisibility(ESlateVisibility::Visible);
			SpinBox_OffsetFromCenter->SetVisibility(ESlateVisibility::Visible);
			CheckBox_DrawDebugLine->SetVisibility(ESlateVisibility::Visible);

			GetWorld()->GetTimerManager().SetTimer(m_hDelayTimer, [pShowSpellRangePatternAction]() {
				pShowSpellRangePatternAction->ShowSpellRangePattern();
				}, 0.1f, false);
		}		
	}
}
