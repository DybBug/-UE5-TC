// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionButton.h"
#include "ShowSpellRangePatternButton.generated.h"

class UVector2DWithNameSpinBox;
class UComboBoxString;
/**
 * 
 */
UCLASS()
class TACTICALCOMBAT_API UShowSpellRangePatternButton : public UActionButton
{
	GENERATED_BODY()
	
public:
	virtual void NativePreConstruct() override;
protected:
	virtual void NativeConstruct() override;


protected:
#pragma region Widgets
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UComboBoxString> ComboBoxString;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UVector2DWithNameSpinBox> SpinBox_Range;
#pragma endregion
	
	FTimerHandle m_hDelayTimer;


protected:
	virtual void OnSelectedActionsChanged(const UAbstractAction* const _leftClickAction, const UAbstractAction* const _rightClickAction) override;

private:
	UFUNCTION()
	void _HandleSpellRangePatternComboBoxSelectionChanged(FString _selectedItem, ESelectInfo::Type _selectionType);

	UFUNCTION()
	void _HandleRangeChanged(const FVector2D& _value);

	void _SetSpellRangePattern();
};
