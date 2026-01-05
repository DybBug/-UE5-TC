// Fill out your copyright notice in the Description page of Project Settings.


#include "Action/Pathfinding/FindPathToTargetAction.h"
#include "Core/PlayerActions.h"
#include "Grid/Grid.h"
#include "Grid/GridPathfinding.h"

void UFindPathToTargetAction::Execute(const FIntPoint& _index)
{
	if (!m_PlayerActions.IsValid())
		return;
	
	AGrid* const pGrid =  m_PlayerActions->GetGrid();
	pGrid->ClearStateFromTiles(ETileStateFlags::InPath);

	pGrid->GetGridPathfinding()->OnPathfindingCompleted.RemoveAll(this);
	pGrid->GetGridPathfinding()->OnPathfindingCompleted.AddUObject(this, &UFindPathToTargetAction::_HandlePathfindingCompleted);

	FIntPoint start = m_PlayerActions->GetSelectedTileIndex();
	FIntPoint target = _index;

	uint8 validTileTypeMask = static_cast<uint8>(ETileType::Normal)
							| static_cast<uint8>(ETileType::DoubleCost)
							| static_cast<uint8>(ETileType::TripleCost);

	
	if (m_bCanUseFlyingOnly)
	{
		validTileTypeMask = validTileTypeMask | static_cast<uint8>(ETileType::FlyingUnitsOnly);
	}
	
	TArray<FIntPoint> neighborIndices =  pGrid->GetGridPathfinding()->FindPathWithNotify(start, target, m_bCanUseDiagonals, validTileTypeMask, m_DelayBetweenIterations, m_MaxMs);
}

void UFindPathToTargetAction::_HandlePathfindingCompleted(const TArray<FIntPoint>& _path)
{
	AGrid* const pGrid =  m_PlayerActions->GetGrid();
	for (const FIntPoint& neighborIndex : _path)
	{
		pGrid->AddStateToTileWithNotify(neighborIndex, ETileStateFlags::InPath);
	}
}
