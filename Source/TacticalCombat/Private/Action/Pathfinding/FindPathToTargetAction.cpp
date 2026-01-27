// Fill out your copyright notice in the Description page of Project Settings.


#include "Action/Pathfinding/FindPathToTargetAction.h"
#include "Core/PlayerActions.h"
#include "Grid/Grid.h"
#include "Grid/GridPathfinding.h"

void UFindPathToTargetAction::Initialize(APlayerActions* const _pPlayerActions)
{
	Super::Initialize(_pPlayerActions);
	_pPlayerActions->OnSelectedTileChanged.AddUObject(this, &UFindPathToTargetAction::_HandleSelectedTileChanged);
}

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

	FPathFindingOptions options = {
		.bIsDiagonalIncluded = m_bCanUseDiagonals,
		.ValidTileTypeMask = static_cast<uint8>(ETileType::Normal)
							| static_cast<uint8>(ETileType::DoubleCost)
							| static_cast<uint8>(ETileType::TripleCost),
		.DelayBetweenIterations = m_DelayBetweenIterations,
		.MaxMsPerFrame = m_MaxMs,
		.bIsReturnReachableTiles = true,
		.MaxPathLength = m_MaxPathLength
	};

	if (m_bCanUseFlyingOnly)
	{
		options.ValidTileTypeMask = options.ValidTileTypeMask | static_cast<uint8>(ETileType::FlyingUnitsOnly);
	};
	
	TArray<FIntPoint> neighborIndices =  pGrid->GetGridPathfinding()->FindPathWithNotify(start, target, options);
}

void UFindPathToTargetAction::_HandlePathfindingCompleted(const TArray<FIntPoint>& _path)
{
	AGrid* const pGrid =  m_PlayerActions->GetGrid();
	for (const FIntPoint& neighborIndex : _path)
	{
		pGrid->AddStateToTileWithNotify(neighborIndex, ETileStateFlags::InPath);
	}
}

void UFindPathToTargetAction::_HandleSelectedTileChanged(const FIntPoint& _index)
{
	Execute(FIntPoint(INVALID_POINT_VALUE));
}
