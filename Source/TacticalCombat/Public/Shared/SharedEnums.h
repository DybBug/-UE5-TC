#pragma once

UENUM(BlueprintType)
enum class EGridShape : uint8
{
	None UMETA(DisplayName = "None"),
	Square UMETA(DisplayName = "Square"),
	Hexagon UMETA(DisplayName = "Hexagon"),
	Triangle UMETA(DisplayName = "Triangle"),
};

UENUM(BlueprintType)
enum class ETileType : uint8
{
	None UMETA(DisplayName = "None"),
	Normal UMETA(DisplayName = "Normal"),
	Obstacle UMETA(DisplayName = "Obstacle"),
	DoubleCost UMETA(DisplayName = "Double Cost"),
	TripleCost UMETA(DisplayName = "Triple Cost"),
	FlyingUnitsOnly UMETA(DisplayName = "Flying Units Only"),
	Max UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(ETileType, ETileType::Max)


UENUM(BlueprintType, Meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class ETileStateFlags : uint8
{
	None = 0		 UMETA(DisplayName = "None"),
	Hovered = 1 << 0 UMETA(DisplayName = "Hovered"),
	Selected = 1 << 1 UMETA(DisplayName = "Selected"),
	Neighbor = 1 << 2 UMETA(DisplayName = "Neighbor"),
	InPath = 1 << 3 UMETA(DisplayName = "InPath"),
	Discovered = 1 << 4 UMETA(DisplayName = "Discovered"),
	Analyzed = 1 << 5 UMETA(DisplayName = "Analyzed"),
};

UENUM(BlueprintType, Meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class ETileDebugFlags : uint8
{
	None = 0			UMETA(DisplayNmae = "None"),
	TileIndices = 1 << 0	UMETA(DisplayNmae = "Tile Indices"),
	CostToEnterTile = 1 << 1	UMETA(DisplayName = "Cost To Enter Tile"),
	MinCostToTarget = 1 << 2	UMETA(DisplayName = "Minimum Cost To Target"),
	CostFromStart = 1 << 3	UMETA(DisplayName = "Cost From Start"),
	SortOrder = 1 << 4	UMETA(DisplayName = "Sort Order"),
	DiscoveredTiles = 1 << 5	UMETA(DisplayName = "Discovered Tiles"),
	AnalysedTiles = 1 << 6	UMETA(DisplayName = "Analysed Tiles"),
	TileType = 1 << 7	UMETA(DisplayName = "Tile Type"),
};

UENUM(BlueprintType)
enum class EUnitAnimationState : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Walk UMETA(DisplayName = "Walk"),
	Attack UMETA(DisplayName = "Attack"),
	Hit UMETA(DisplayName = "Hit"),
	Death UMETA(DisplayName = "Death"),
	Respawn UMETA(DisplayName = "Respawn"),
};