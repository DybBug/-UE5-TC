// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UtilityLibrary.generated.h"

/**
 * 
 */
UCLASS()
class TACTICALCOMBAT_API UUtilityLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION()
	static FVector SnapVectorToVector(const FVector& _lhs, const FVector& _rhs);
	
	UFUNCTION()
	static bool IsFloatEven(float _value);

	UFUNCTION()
	static bool IsIntEven(int32 _value);

};
