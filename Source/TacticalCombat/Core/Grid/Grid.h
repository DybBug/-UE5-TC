// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridModifier.h"
#include "GridVisual.h"
#include "GameFramework/Actor.h"
#include "TacticalCombat/Structure/GridDatas.h"
#include "Grid.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnTileDataUpdated, const FIntPoint&);
DECLARE_MULTICAST_DELEGATE(FOnGridDestroyed);

class AGridVisual;
class AGridPathfinding;
class UInstancedStaticMeshComponent;
enum class ETileType : uint8;
enum class EGridShape : uint8;

UCLASS()
class TACTICALCOMBAT_API AGrid : public AActor
{
	GENERATED_BODY()

public:
	static bool IsWalkableTile(ETileType _type);
	
public:	
	// Sets default values for this actor's properties
	AGrid();
	
	
protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float _deltaTime) override;

	UFUNCTION()
	void SpawnGrid(
		const FVector& _centerLocation,
		const FVector& _tileSize,
		const FIntPoint& _tileCount,
		EGridShape _shape,
		bool _bIsUseEnvironment = true);

	UFUNCTION()
	void DestroyGrid();
	
	FGridShapeData GetGridShapeData() const;

	FVector GetCursorLocationOnGrid(int _playerIndex);
	FIntPoint GetTileIndexFromWorldLocation(const FVector& _location);
	FIntPoint GetTileIndexUnderCursor(int _playerIndex);
	void AddGridTile(const FTileData& _tileData);
	void RemoveGridTile(const FIntPoint& _tileIndex);
	void AddStateToTile(const FIntPoint& _tileIndex, const ETileStateFlags _stateFlag);
	void RemoveStateFromTile(const FIntPoint& _tileIndex, const ETileStateFlags _stateFlag);
	bool IsIndexValid(const FIntPoint& _tileIndex);
	FVector TraceForGround(const FVector& _location, ETileType& _hitTileType);

	FVector  GetTileLocationFromGridIndex(int _row, int _col);
	FRotator GetTileRotationFromGridIndex(int _row, int _col);
	FVector GetTileScale();

	TArray<FIntPoint> GetAllTilesWithStateFlag(const ETileStateFlags _stateFlag);
	void ClearStateFromTiles(const ETileStateFlags _stateFlag);

	bool IsWalkableTile(const FIntPoint& _index);

#pragma region Getter
	FORCEINLINE const FVector& GetCenterLocation() const { return m_CenterLocation; }
	FORCEINLINE const FVector& GetTileSize() const { return m_TileSize; }
	FORCEINLINE const FIntPoint& GetTileCount() const { return m_TileCount; }
	FORCEINLINE EGridShape GetGridShape() const { return m_Shape; }
	FORCEINLINE const FVector& GetBottomLeftLocation() const { return m_GridBottomLeftCornerLocation; }
	FORCEINLINE float GetZOffset() { return m_GridVisual != nullptr ? m_GridVisual->GetZOffset() : 0.0f; }
	FORCEINLINE const TMap<FIntPoint, FTileData>& GetGridTileMap() const { return m_GridTileMap; }
	FORCEINLINE AGridPathfinding* GetGridPathfinding() const { return m_GridPathfinding.Get(); }

#pragma endregion
	

#pragma region Setter
	FORCEINLINE void SetZOffset(float _offset) {
		if (m_GridVisual)
		{
			m_GridVisual->SetZOffset(_offset);
		}
	}
#pragma endregion

public:
	FOnTileDataUpdated OnTileDataUpdated;
	FOnGridDestroyed OnGridDestroyed;
	
protected:
#pragma region Component
	UPROPERTY(EditAnywhere, Category = "Component", Meta = (DisplayName = "Root"))
	TObjectPtr<USceneComponent> m_SceneComponent;

	UPROPERTY(EditAnywhere, Category = "Component", Meta = (DisplayName = "Grid Visual"))
	TObjectPtr<UChildActorComponent> m_ChildActorGridVisual;
	
	UPROPERTY(EditAnywhere, Category = "Component", Meta = (DisplayName = "Grid Pathfinding"))
	TObjectPtr<UChildActorComponent> m_ChildActorGridPathFinding;
#pragma endregion

#pragma region Properties
	UPROPERTY(EditAnywhere, Category = "Property", Meta = (DisplayName = "Center Location"))
	FVector m_CenterLocation;

	UPROPERTY(EditAnywhere, Category = "Property", Meta = (DisplayName = "Tile Size"))
	FVector m_TileSize = FVector(200.0f, 200.0f, 100.0f);;

	UPROPERTY(EditAnywhere, Category = "Property", Meta = (DisplayName = "Tile Count"))
	FIntPoint m_TileCount = FIntPoint(10, 10);

	UPROPERTY(EditAnywhere, Category = "Property", Meta = (DisplayName = "Shape Type"))
	EGridShape m_Shape = EGridShape::Square;

	UPROPERTY(VisibleAnywhere, Category = "Property", Meta = (DisplayName = "Grid Bottom Left Corner Location"))
	FVector m_GridBottomLeftCornerLocation;	

	UPROPERTY(VisibleAnywhere, Category = "Property", Meta = (DisplayName = "Grid Tile Map"))
	TMap<FIntPoint, FTileData> m_GridTileMap;

	UPROPERTY(EditAnywhere, Category = "Property", Meta = (DisplayName = "Grid Visual"))
	TObjectPtr<AGridVisual> m_GridVisual;

	UPROPERTY(EditAnywhere, Category = "Property", Meta = (DisplayName = "Grid Pathfinding"))
	TObjectPtr<AGridPathfinding> m_GridPathfinding;
#pragma endregion

#pragma region Internals
	TMap<ETileStateFlags, TArray<FIntPoint>> m_TileIndicesByStateFlag;
#pragma endregion
	


private:
	void _CalculateCenterAndBottomLeft(FVector& _center, FVector& _bottomLeft);
};
