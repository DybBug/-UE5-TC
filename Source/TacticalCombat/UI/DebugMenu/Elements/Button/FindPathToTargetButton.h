// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionButton.h"
#include "FindPathToTargetButton.generated.h"

class UWithNameSpinBox;
class UCheckBox;
class UAbstractAction;
/**
 * 
 */
UCLASS()
class TACTICALCOMBAT_API UFindPathToTargetButton : public UActionButton
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
public:
	virtual void NativePreConstruct() override;

protected:
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UCheckBox> CheckBox_CanUseDiagonals;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UCheckBox> CheckBox_CanUseFlyingOnly;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UWithNameSpinBox> SpinBox_Delay;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UWithNameSpinBox> SpinBox_MaxMs;
protected:
	virtual void OnSelectedActionsChanged(const UAbstractAction* const _leftClickAction, const UAbstractAction* const _rightClickAction) override;

	UFUNCTION()
	virtual void OnCanUseDiagonalsCheckBoxStateChanged(bool _bIsChecked);

	UFUNCTION()
	virtual void OnCanUseFlyingOnlyCheckBoxStateChanged(bool _bIsChecked);

	UFUNCTION()
	virtual void OnDelaySpinBoxValueChanged(float _value);
	
	UFUNCTION()
	virtual void OnMaxMsSpinBoxValueChanged(float _value);

private:
	void _UpdateAllElementVisibility(bool _isVisible);
	
};
