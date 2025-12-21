// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Table/Rows/UnitAssetsTableRow.h"
#include "UnitsLibrary.generated.h"

enum class EUnitType: uint8;
/**
 * 
 */
UCLASS()
class TACTICALCOMBAT_API UUnitsLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static FUnitAssetsTableRow GetDefaultUnitAssetsDataFromUnitType(ETacticalUnitType _unitType);
};
