// Fill out your copyright notice in the Description page of Project Settings.


#include "ShowTileNeighborsAction.h"

#include "TacticalCombat/Core/Grid/Grid.h"
#include "TacticalCombat/Core/Grid/GridPathfinding.h"

void UShowTileNeighborsAction::Execute(const FIntPoint& _index)
{
	if (m_PlayerActions == nullptr)
		return;
	
	AGrid* const pGrid =  m_PlayerActions->GetGrid();
	pGrid->ClearStateFromTiles(ETileStateFlags::Neighbor);

	TArray<ETileType> validTileTypes =
	{
		ETileType::Normal,
		ETileType::DoubleCost,
		ETileType::TripleCost,
		ETileType::FlyingUnitsOnly
	};
	
	TArray<FPathfindingNode> neighborNodes = pGrid->GetGridPathfinding()->GetValidTileNeighborNodes(_index, m_bUseDiagonals, validTileTypes);
	for (const FPathfindingNode& neighborNode : neighborNodes)
	{
		pGrid->AddStateToTile(neighborNode.Index, ETileStateFlags::Neighbor);
	}
}
