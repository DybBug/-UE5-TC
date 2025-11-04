// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridPathfinding.generated.h"

class AGrid;

UCLASS()
class TACTICALCOMBAT_API AGridPathfinding : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridPathfinding();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	TArray<FIntPoint> GetValidTileNeighbors(const FIntPoint& _index, bool _bUseDiagonals = false);
	TArray<FIntPoint> GetNeighborIndices(const FIntPoint& _index, bool _bUseDiagonals = false);
	

#pragma region Setter
	FORCEINLINE void SetGrid(AGrid* const _pGrid) { m_Grid = _pGrid; }
#pragma endregion

private:
#pragma region Internals
	UPROPERTY(VisibleInstanceOnly, Category = "Internal", Meta = (DisplayName = "Grid"))
	TWeakObjectPtr<AGrid> m_Grid;
#pragma endregion

private:
	TArray<FIntPoint> _GetNeighborIndicesForSquare(const FIntPoint& _index, bool _bUseDiagonals);
	TArray<FIntPoint> _GetNeighborIndicesForHexagon(const FIntPoint& _index);
	TArray<FIntPoint> _GetNeighborIndicesForTriangle(const FIntPoint& _index, bool _bUseDiagonals);

};
