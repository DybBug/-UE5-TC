// Fill out your copyright notice in the Description page of Project Settings.


#include "ShowTileNeighborsAction.h"

#include "TacticalCombat/Core/PlayerActions.h"
#include "TacticalCombat/Core/Grid/Grid.h"
#include "TacticalCombat/Core/Grid/GridPathfinding.h"

void UShowTileNeighborsAction::Execute(const FIntPoint& _index)
{
	AGrid* const pGrid =  m_PlayerActions->GetGrid();
	pGrid->ClearStateFromTiles(ETileStateFlags::Neighbor);
	TArray<FIntPoint> neighborIndices =  pGrid->GetGridPathfinding()->GetValidTileNeighbors(_index, m_bUseDiagonals);
	for (FIntPoint index : neighborIndices)
	{
		pGrid->AddStateToTile(index, ETileStateFlags::Neighbor);
	}
}
