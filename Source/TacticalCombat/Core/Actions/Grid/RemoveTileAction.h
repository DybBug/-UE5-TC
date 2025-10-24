// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../AbstractAction.h"
#include "RemoveTileAction.generated.h"

/**
 * 
 */
UCLASS()
class TACTICALCOMBAT_API URemoveTileAction : public UAbstractAction
{
	GENERATED_BODY()

public:
	virtual void Execute(const FIntPoint& _index) override;
};
