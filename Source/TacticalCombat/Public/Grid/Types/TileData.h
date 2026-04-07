#pragma once
#include "CoreMinimal.h"
#include "TileData.generated.h"

class AUnit;

UENUM(BlueprintType, Meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class ETileType : uint8
{
	None = 0					UMETA(DisplayName = "None"),
	Normal = 1 << 0				UMETA(DisplayName = "Normal"),
	Obstacle = 1 << 1			UMETA(DisplayName = "Obstacle"),
	DoubleCost = 1 << 2			UMETA(DisplayName = "Double Cost"),
	TripleCost = 1 << 3			UMETA(DisplayName = "Triple Cost"),
	FlyingUnitsOnly = 1 << 4	UMETA(DisplayName = "Flying Units Only"),
	InSpellRange = 1 << 5		UMETA(DisplayName = "In Spell Range"),
	Max UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(ETileType, ETileType::Max)

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
	uint8 StateFlags;
	
	UPROPERTY(EditAnywhere, blueprintReadWrite)
	TWeakObjectPtr<AUnit> UnitOnTile;

};