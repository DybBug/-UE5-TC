#pragma once

#include "CoreMinimal.h"
#include "Unit/Types/UnitAssets.h"
#include "Unit/Types/UnitStats.h"
#include "UnitTableRow.generated.h"

enum class ETacticalUnitType: uint8;

USTRUCT(BlueprintType)
struct FUnitTableRow : public FTableRowBase
{
	GENERATED_BODY()

public :
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETacticalUnitType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FUnitAssets Assets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FUnitStats Stats;
};
