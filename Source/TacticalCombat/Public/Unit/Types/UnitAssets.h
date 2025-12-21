#pragma once

#include "CoreMinimal.h"
#include "UnitAssets.generated.h"

class USkeletalMesh;
class UAnimationAsset;

USTRUCT(BlueprintType)
struct FUnitAssets
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMesh> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UAnimInstance> AnimationBPClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTexture2D> Icon;
	
};
