// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbstractAction.h"
#include "AddTileAction.generated.h"

/**
 * 
 */
UCLASS()
class TACTICALCOMBAT_API UAddTileAction : public UAbstractAction
{
	GENERATED_BODY()

public:
	virtual void Execute(const FIntPoint& _index) override;
};
