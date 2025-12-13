// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionButton.h"
#include "ModifyTileHeightButton.generated.h"

class APlayerActions;
/**
 * 
 */
UCLASS()
class TACTICALCOMBAT_API UModifyTileHeightButton : public UActionButton
{
	GENERATED_BODY()

private:
	virtual void OnSelectedActionsChanged(const UAbstractAction* const _leftClickAction, const UAbstractAction* const rightClickAction) override;
};
