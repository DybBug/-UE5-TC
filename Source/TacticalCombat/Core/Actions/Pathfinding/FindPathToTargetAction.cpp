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

	pGrid->GetGridPathfinding()->OnPathfindingCompleted.AddUObject(this, &UFindPathToTargetAction::_OnPathfindingCompleted);

	FIntPoint start = m_PlayerActions->GetSelectedTileIndex();
	FIntPoint target = _index;
	TArray<FIntPoint> neighborIndices =  pGrid->GetGridPathfinding()->FindPath(start, target, m_bUseDiagonals, m_DelayBetweenIterations, m_MaxMs);

}

void UFindPathToTargetAction::_OnPathfindingCompleted(const TArray<FIntPoint>& _path)
{
	AGrid* const pGrid =  m_PlayerActions->GetGrid();
	for (const FIntPoint& neighborIndex : _path)
	{
		pGrid->AddStateToTile(neighborIndex, ETileStateFlags::InPath);
	}
}
