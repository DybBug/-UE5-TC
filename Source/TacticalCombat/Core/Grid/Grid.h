// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grid.generated.h"

class UInstancedStaticMeshComponent;
enum class EGridShape : uint8;

UCLASS()
class TACTICALCOMBAT_API AGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrid();

	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float _deltaTime) override;

	UFUNCTION()
	void SpawnGrid(const FVector& _centerLocation, const FVector& _tileSize, const FVector2D& _tileCount, EGridShape _shape);

	UFUNCTION()
	void DestroyGrid();

#pragma region Getter
	FORCEINLINE const FVector& GetCenterLocation() const { return m_CenterLocation; }
	FORCEINLINE const FVector& GetTileSize() const { return m_TileSize; }
	FORCEINLINE const FVector2D& GetTileCount() const { return m_TileCount; }
	FORCEINLINE EGridShape GetGridShape() const { return m_Shape; }
	FORCEINLINE const FVector& GetBottomLeftLocation() const { return m_GridBottomLeftCornerLocation; }
#pragma endregion
	
protected:
#pragma region Component
	UPROPERTY(EditAnywhere, Category = "Component", Meta = (DisplayName = "Root"))
	TObjectPtr<USceneComponent> m_SceneComp;
	
	UPROPERTY(EditAnywhere, Category = "Component", Meta = (DisplayName = "Instanced Static Mesh"))
	TObjectPtr<UInstancedStaticMeshComponent> m_InstancedStaticMeshComp;
#pragma endregion

#pragma region Properties
	UPROPERTY(EditAnywhere, Category = "Property", Meta = (DisplayName = "Center Location"))
	FVector m_CenterLocation;

	UPROPERTY(EditAnywhere, Category = "Property", Meta = (DisplayName = "Tile Size"))
	FVector m_TileSize = FVector(200.0f, 200.0f, 100.0f);;

	UPROPERTY(EditAnywhere, Category = "Property", Meta = (DisplayName = "Tile Count"))
	FVector2D m_TileCount = FVector2D(10.0f, 10.0f);

	UPROPERTY(EditAnywhere, Category = "Property", Meta = (DisplayName = "Shape Type"))
	EGridShape m_Shape;

	UPROPERTY(VisibleAnywhere, Category = "Property", Meta = (DisplayName = "Grid Bottom Left Corner Location"))
	FVector m_GridBottomLeftCornerLocation;
#pragma region
	UPROPERTY()
	TObjectPtr<UDataTable> m_GridShapeDataTable;

protected:	
	void CalculateCenterAndBottomLeft(FVector& _center, FVector& _bottomLeft);

private:
	FVector  _GetTileLocationFromGridIndex(int _row, int _col);
	FRotator _GetTileRotationFromGridIndex(int _row, int _col);

};
