// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../AbstractAction.h"
#include "SelectTileAction.generated.h"

/**
 * 
 */
UCLASS()
class TACTICALCOMBAT_API USelectTileAction : public UAbstractAction
{
	GENERATED_BODY()

public:
	virtual void BeginDestroy() override;
	
	virtual void Execute(const FIntPoint& _index) override;
	
};
