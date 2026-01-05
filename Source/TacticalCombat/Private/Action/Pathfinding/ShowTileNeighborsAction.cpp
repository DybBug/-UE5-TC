// Fill out your copyright notice in the Description page of Project Settings.


#include "Action/Pathfinding/ShowTileNeighborsAction.h"

#include "Core/PlayerActions.h"
#include "Grid/Grid.h"
#include "Grid/GridPathfinding.h"
#include "Grid/Types/PathfindingNode.h"

void UShowTileNeighborsAction::Execute(const FIntPoint& _index)
{
	if (m_PlayerActions == nullptr)
		return;
	
	AGrid* const pGrid =  m_PlayerActions->GetGrid();
	pGrid->ClearStateFromTiles(ETileStateFlags::Neighbor);

	uint8 validTileTypeMask = static_cast<uint8>(ETileType::Normal)
								| static_cast<uint8>(ETileType::DoubleCost)
								| static_cast<uint8>(ETileType::TripleCost)
								| static_cast<uint8>(ETileType::FlyingUnitsOnly);
	
	TArray<FPathfindingNode> neighborNodes = pGrid->GetGridPathfinding()->GetValidTileNeighborNodes(_index, m_bUseDiagonals, validTileTypeMask);
	for (const FPathfindingNode& neighborNode : neighborNodes)
	{
		pGrid->AddStateToTileWithNotify(neighborNode.Index, ETileStateFlags::Neighbor);
	}
}
