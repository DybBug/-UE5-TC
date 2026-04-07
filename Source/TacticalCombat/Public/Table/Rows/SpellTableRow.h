#pragma once
#include "CoreMinimal.h"
#include "Combat/Spells/Types/SpellAssets.h"
#include "Combat/Spells/Types/SpellCast.h"
#include "SpellTableRow.generated.h"

UENUM(BlueprintType)
enum class ESpellType: uint8
{
	None UMETA(DisplayName = "None"),
	Test UMETA(DisplayName = "Test"),
	Max UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(ESpellType, ESpellType::Max)

USTRUCT(BlueprintType)
struct FSpellTableRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESpellType SpellType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSpellAssets SpellAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSpellCast SpellCast;
	
};
