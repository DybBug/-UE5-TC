// Fill out your copyright notice in the Description page of Project Settings.


#include "Action/Pathfinding/SelectTileWithCalculateMinCost.h"

#include "Core/PlayerActions.h"
#include "Grid/Grid.h"
#include "Grid/GridPathfinding.h"
#include "Grid/Types/PathfindingNode.h"

void USelectTileWithCalculateMinCost::Execute(const FIntPoint& _index)
{
	Super::Execute(_index);
	if (!m_PlayerActions.IsValid())
		return;

	AGrid* pGrid = m_PlayerActions->GetGrid();
	AGridPathfinding* pGridPathfinding = pGrid->GetGridPathfinding();
	
	pGridPathfinding->ClearGeneratedData();

	TArray<FIntPoint> path;
	pGrid->GetGridTileMap().GetKeys(path);
	for (const FIntPoint& index : path)
	{
		const FIntPoint& selectedIndex = m_PlayerActions->GetSelectedTileIndex();
		int32 minimumCostToTarget = pGridPathfinding->GetMinimumCostBetweenTwoNodes(index, selectedIndex, m_bIsDiagonalIncluded);

		FPathfindingNode pathfindingNode;
		pathfindingNode.Index = index;
		pathfindingNode.CostToEnterTile = 1;
		pathfindingNode.CostFromStart = 0;
		pathfindingNode.MinimumCostToTarget = minimumCostToTarget;
		pGridPathfinding->DiscoverNode(pathfindingNode);
	}
	
}
