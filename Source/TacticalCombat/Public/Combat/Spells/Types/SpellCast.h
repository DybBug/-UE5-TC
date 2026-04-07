#pragma once
#include "SpellCast.generated.h"

UENUM(BlueprintType)
enum class ESpellRangePattern: uint8
{
	None			UMETA(DisplayName = "None"),
	Line			UMETA(DisplayName = "Line"),
	Diagonal		UMETA(DisplayName = "Diagonal"),
	HalfDiagonal	UMETA(DisplayName = "HalfDiagonal"),
	Star			UMETA(DisplayName = "Start"),
	Diamond			UMETA(DisplayName = "Diamond"),
	Square			UMETA(DisplayName = "Square"),
	Max				UMETA(Hidden),
};
ENUM_RANGE_BY_COUNT(ESpellRangePattern, ESpellRangePattern::Max);


USTRUCT(BlueprintType)
struct FSpellCast
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESpellRangePattern RangePattern;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntPoint RangeBounds;
};
