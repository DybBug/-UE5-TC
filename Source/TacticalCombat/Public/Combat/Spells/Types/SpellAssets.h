#pragma once
#include "SpellAssets.generated.h"

class ASpellAnimation;

USTRUCT(BlueprintType)
struct FSpellAssets
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ASpellAnimation> SpellAnimationClass;
};
