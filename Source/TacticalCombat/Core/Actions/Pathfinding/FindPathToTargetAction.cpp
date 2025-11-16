// Fill out your copyright notice in the Description page of Project Settings.


#include "FindPathToTargetAction.h"
#include "TacticalCombat/Core/Grid/Grid.h"
#include "TacticalCombat/Core/Grid/GridPathfinding.h"

void UFindPathToTargetAction::Execute(const FIntPoint& _index)
{
	if (!m_PlayerActions.IsValid())
		return;
	
	AGrid* const pGrid =  m_PlayerActions->GetGrid();
	pGrid->ClearStateFromTiles(ETileStateFlags::InPath);

	FIntPoint start = m_PlayerActions->GetSelectedTileIndex();
	FIntPoint target = _index;
	TArray<FIntPoint> neighborIndices =  pGrid->GetGridPathfinding()->FindPath(start, target, m_bUseDiagonals);
	for (const FIntPoint& neighborIndex : neighborIndices)
	{
		pGrid->AddStateToTile(neighborIndex, ETileStateFlags::InPath);
	}
}
