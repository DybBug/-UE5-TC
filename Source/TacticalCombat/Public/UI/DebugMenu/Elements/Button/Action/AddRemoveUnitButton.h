// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/DebugMenu/Elements/Button/Action/ActionButton.h"
#include "AddRemoveUnitButton.generated.h"

class UUnitButtonList;
enum class ETacticalUnitType : uint8;
/**
 * 
 */
UCLASS()
class TACTICALCOMBAT_API UAddRemoveUnitButton : public UActionButton
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void OnSelectedActionsChanged(const UAbstractAction* const _leftClickAction, const UAbstractAction* const rightClickAction) override;

protected:
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UUnitButtonList> Button_UnitList;
	
private:
	void _UpdateWidget(ETacticalUnitType _unitType);
	
	UFUNCTION()
	void _OnSelectedUnitButtonChanged(ETacticalUnitType _unitType);
};

