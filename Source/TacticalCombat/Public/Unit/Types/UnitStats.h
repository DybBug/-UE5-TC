#pragma once
#include "UnitStats.generated.h"

enum class ETileType: uint8;

USTRUCT(BlueprintType)
struct FUnitStats
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (BitMask, BitmaskEnum = "/Script/TacticalCombat.ETileType"))  
	uint8 ValidTileTypeMask;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanDiagonalMove;
};
