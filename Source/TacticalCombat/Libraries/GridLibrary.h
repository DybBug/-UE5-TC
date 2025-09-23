// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GridLibrary.generated.h"

enum class EGridShape: uint8;
/**
 * 
 */
UCLASS()
class TACTICALCOMBAT_API UGridLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "GridLibrary")
	static FGridShapeData GetGridShape(EGridShape _gridShape);
};
