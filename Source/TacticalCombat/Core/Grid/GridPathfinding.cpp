// Fill out your copyright notice in the Description page of Project Settings.


#include "GridPathfinding.h"

#include "Grid.h"
#include "TacticalCombat/Libraries/UtilityLibrary.h"

// Sets default values
AGridPathfinding::AGridPathfinding()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AGridPathfinding::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGridPathfinding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

TArray<FIntPoint> AGridPathfinding::GetValidTileNeighbors(const FIntPoint& _index, bool _bUseDiagonals)
{
	TArray<FIntPoint> neighborIndices;
	const FTileData* pInputTileData = m_Grid->GetGridTileMap().Find(_index);
	if (!pInputTileData)
	{
		return neighborIndices;
	}
	 
	TArray<FIntPoint> tempNeighborIndices = GetNeighborIndices(_index, _bUseDiagonals);
	for (const FIntPoint& tempNeighborIndex : tempNeighborIndices)
	{
		const FTileData* pTempNeighborTileData = m_Grid->GetGridTileMap().Find(tempNeighborIndex);
		if (pTempNeighborTileData && AGrid::IsWalkableTile(pTempNeighborTileData->Type))
		{
			float inputTileHeight = pInputTileData->Transform.GetLocation().Z;
			float tempNeighborTileHeight = pTempNeighborTileData->Transform.GetLocation().Z;
			if (FMath::Abs(inputTileHeight - tempNeighborTileHeight) <= m_Grid->GetTileSize().Z)
			{
				neighborIndices.Add(tempNeighborIndex);
			}
		}
	}
	return neighborIndices;
}

TArray<FIntPoint> AGridPathfinding::GetNeighborIndices(const FIntPoint& _index, bool _bUseDiagonals)
{
	switch (m_Grid->GetGridShape())
	{
		case EGridShape::None:		return TArray<FIntPoint>();
		case EGridShape::Square:	return _GetNeighborIndicesForSquare(_index, _bUseDiagonals);
		case EGridShape::Hexagon:	return _GetNeighborIndicesForHexagon(_index);
		case EGridShape::Triangle:	return _GetNeighborIndicesForTriangle(_index, _bUseDiagonals);
		default:					return TArray<FIntPoint>();
	}
}

TArray<FIntPoint> AGridPathfinding::_GetNeighborIndicesForSquare(const FIntPoint& _index, bool _bUseDiagonals)
{
	TArray <FIntPoint> neighbors;
	neighbors.Add(_index + FIntPoint(+1, +0)); // top
	neighbors.Add(_index + FIntPoint(+0, +1)); // right
	neighbors.Add(_index + FIntPoint(-1, +0)); // bottom
	neighbors.Add(_index + FIntPoint(+0, -1)); // left
	if (_bUseDiagonals)
	{
		neighbors.Add(_index + FIntPoint(+1, -1)); // left_top
		neighbors.Add(_index + FIntPoint(+1, +1)); // right_top
		neighbors.Add(_index + FIntPoint(-1, +1)); // right_bottom
		neighbors.Add(_index + FIntPoint(-1, -1)); // left_bottom
	}
	return neighbors;
}

TArray<FIntPoint> AGridPathfinding::_GetNeighborIndicesForHexagon(const FIntPoint& _index)
{
	TArray<FIntPoint> neighbors;
	neighbors.Add(_index + FIntPoint(+1, -1));
	neighbors.Add(_index + FIntPoint(+1, +1));
	neighbors.Add(_index + FIntPoint(+0, +2));
	neighbors.Add(_index + FIntPoint(-1, +1));
	neighbors.Add(_index + FIntPoint(-1, -1));
	neighbors.Add(_index + FIntPoint(+0, -2));
	neighbors.Add(_index + FIntPoint(+1, -1));
	return neighbors;
}

TArray<FIntPoint> AGridPathfinding::_GetNeighborIndicesForTriangle(const FIntPoint& _index, bool _bUseDiagonals)
{
	TArray<FIntPoint> neighbors;

	neighbors.Add(_index + FIntPoint(0, -1));
	neighbors.Add(_index + FIntPoint(0, 1));

	bool bIsUpwardTriangle = UUtilityLibrary::IsIntEven(_index.X) == UUtilityLibrary::IsIntEven(_index.Y);
	if (bIsUpwardTriangle)
	{
		neighbors.Add(_index + FIntPoint(-1, 0));
	}
	else
	{
		neighbors.Add(_index + FIntPoint(1, 0));
	}

	if (_bUseDiagonals)
	{
		if (bIsUpwardTriangle)
		{
			neighbors.Add(_index + FIntPoint(1, 0));
			neighbors.Add(_index + FIntPoint(-1, 2));
			neighbors.Add(_index + FIntPoint(-1, -2));
		}
		else
		{
			neighbors.Add(_index + FIntPoint(-1, 0));
			neighbors.Add(_index + FIntPoint(1, 2));
			neighbors.Add(_index + FIntPoint(1, -2));
		}
	}
	
	return neighbors;
}



