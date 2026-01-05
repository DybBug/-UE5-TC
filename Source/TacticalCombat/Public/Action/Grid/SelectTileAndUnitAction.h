// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action/AbstractAction.h"
#include "SelectTileAndUnitAction.generated.h"

/**
 * 
 */
UCLASS()
class TACTICALCOMBAT_API USelectTileAndUnitAction : public UAbstractAction
{
	GENERATED_BODY()

public:	
	virtual void Execute(const FIntPoint& _index) override;
	
};
