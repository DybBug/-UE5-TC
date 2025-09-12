#pragma once
#include "Engine/DataTable.h"
#include "GridShapeData.generated.h"

class UStaticMesh;
class UMaterialInstance;

UENUM(BlueprintType)
enum class EGridShape : uint8
{
    None UMETA(DisplayName = "None"),
	Square UMETA(DisplayName = "Square"),
	Hexagon UMETA(DisplayName = "Hexagon"),
	Triangle UMETA(DisplayName = "Triangle"),
};

USTRUCT(BlueprintType)
struct FGridShapeData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector MeshSize = FVector(100.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMesh> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UMaterialInstance> MeshMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMesh> FlatMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UMaterialInstance> FlatBoardMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UMaterialInstance> FlatFilledMaterial;
};