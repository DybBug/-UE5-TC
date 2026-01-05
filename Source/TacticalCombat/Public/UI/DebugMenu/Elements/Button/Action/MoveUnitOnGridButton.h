// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionButton.h"
#include "MoveUnitOnGridButton.generated.h"

class UWithNameSpinBox;
/**
 * 
 */
UCLASS()
class TACTICALCOMBAT_API UMoveUnitOnGridButton : public UActionButton
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

protected:
#pragma region Widgets
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UWithNameSpinBox> SpinBox_MoveDurationPerTile;
#pragma endregion

protected:
	virtual void OnSelectedActionsChanged(const UAbstractAction* const _leftClickAction, const UAbstractAction* const _rightClickAction) override;

private:
	void _UpdateSpinBoxMaxMsVisual();
	
	UFUNCTION()
	void _HandleMaxMsValueChaged(float _value);
	
	
};
