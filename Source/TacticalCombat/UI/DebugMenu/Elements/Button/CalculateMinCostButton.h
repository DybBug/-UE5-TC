// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TacticalCombat/UI/DebugMenu/Elements/Button/ActionButton.h"
#include "CalculateMinCostButton.generated.h"

class UCheckBox;

/**
 * 
 */
UCLASS()
class TACTICALCOMBAT_API UCalculateMinCostButton : public UActionButton
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
public:
	virtual void NativePreConstruct() override;

protected:
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UCheckBox> CheckBox_UseDiagonals;

protected:
	virtual void OnSelectedActionsChanged(const UAbstractAction* const _leftClickAction, const UAbstractAction* const _rightClickAction) override;

	UFUNCTION()
	virtual void OnUseDiagonalsCheckBoxStateChanged(bool _bIsChecked);
};
