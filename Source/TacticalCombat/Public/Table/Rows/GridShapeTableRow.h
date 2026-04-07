#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Shared/SharedConstants.h"
#include "GridShapeTableRow.generated.h"

USTRUCT(BlueprintType)
struct FGridShapeTableRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector MeshSize = FVector(Tile::DEFAULT_TILE_SIZE_X, Tile::DEFAULT_TILE_SIZE_Y, Tile::DEFAULT_TILE_SIZE_Z);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMesh> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UMaterialInstance> MeshMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMesh> FlatMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UMaterialInstance> FlatMaterial;
};