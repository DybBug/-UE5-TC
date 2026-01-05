#pragma once
#include "CoreMinimal.h"
#include "Shared/SharedEnums.h"
#include "TileData.generated.h"

class AUnit;

USTRUCT(BlueprintType)
struct FTileData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntPoint Index;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETileType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform Transform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (BitMask, BitmaskEnum = "/Script/TacticalCombat.ETileStateFlags"))  
	uint8 StateMask;
	
	UPROPERTY(EditAnywhere, blueprintReadWrite)
	TWeakObjectPtr<AUnit> UnitOnTile;

};