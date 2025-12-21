#pragma once

#include "CoreMinimal.h"
#include "Unit/Types/UnitAssets.h"
#include "UnitAssetsTableRow.generated.h"

enum class ETacticalUnitType: uint8;

USTRUCT(BlueprintType)
struct FUnitAssetsTableRow : public FTableRowBase
{
	GENERATED_BODY()

public :
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETacticalUnitType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FUnitAssets Assets;
};
