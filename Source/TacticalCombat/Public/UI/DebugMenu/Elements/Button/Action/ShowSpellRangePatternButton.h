// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionButton.h"
#include "ShowSpellRangePatternButton.generated.h"

class UWithNameSpinBox;
class UVector2DWithNameSpinBox;
class UComboBoxString;
class UCheckBox;

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

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UCheckBox> CheckBox_LineOfSight;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UWithNameSpinBox> SpinBox_HeightFromGround;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UWithNameSpinBox> SpinBox_OffsetFromCenter;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr< UCheckBox> CheckBox_DrawDebugLine;
#pragma endregion
	
	FTimerHandle m_hDelayTimer;


protected:
	virtual void OnSelectedActionsChanged(const UAbstractAction* const _leftClickAction, const UAbstractAction* const _rightClickAction) override;

private:
#pragma region Handles
	UFUNCTION()
	void _HandleSpellRangePatternComboBoxSelectionChanged(FString _selectedItem, ESelectInfo::Type _selectionType);

	UFUNCTION()
	void _HandleRangeChanged(const FVector2D& _value);

	UFUNCTION()
	void _HandleLineOfSightCheckBoxComponentStateChanged(bool _bIsChecked);

	UFUNCTION()
	void _HandleHeightFromGroundChanged(float _value);

	UFUNCTION()
	void _HandleOffsetFromCenterChanged(float _value);

	UFUNCTION()
	void _HandleDrawDebugLineChanged(bool _bIsChecked);
#pragma endregion

	void _SetSpellRangePattern();
};
