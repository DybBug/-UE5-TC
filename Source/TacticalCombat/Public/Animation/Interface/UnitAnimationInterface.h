// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UnitAnimationInterface.generated.h"

enum class EUnitAnimationState : uint8;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UUnitAnimationInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TACTICALCOMBAT_API IUnitAnimationInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetUnitAnimationState(EUnitAnimationState _newState);
	virtual void SetUnitAnimationState_Implementation(EUnitAnimationState _newState);
};
