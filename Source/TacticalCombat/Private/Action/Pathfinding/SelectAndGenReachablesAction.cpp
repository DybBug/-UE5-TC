// Fill out your copyright notice in the Description page of Project Settings.


#include "Action/Pathfinding/SelectAndGenReachablesAction.h"

#include "Grid/Grid.h"
#include "Core/PlayerActions.h"
#include "Shared/SharedEnums.h"
#include "Grid/GridPathfinding.h"
#include "Unit/Unit.h"

void USelectAndGenReachablesAction::Execute(const FIntPoint& _index)
{
	Super::Execute(_index);
	_GenerateReachable();	
}

void USelectAndGenReachablesAction::_GenerateReachable()
{
	if (!m_PlayerActions.IsValid())
		return;
	
	AGrid* const pGrid =  m_PlayerActions->GetGrid();
	pGrid->ClearStateFromTiles(ETileStateFlags::Reachable);

	pGrid->GetReachablesPathfinding()->OnPathfindingCompleted.RemoveAll(this);
	pGrid->GetReachablesPathfinding()->OnPathfindingCompleted.AddUObject(this, &USelectAndGenReachablesAction::_HandlePathfindingCompleted);
	
	FIntPoint start;
	FIntPoint target;
	FPathFindingOptions options;
	if (AUnit* pUnit = m_PlayerActions->GetSelectedUnit())
	{
		pUnit->OnUnitStartedWalking.RemoveAll(this);
		pUnit->OnUnitStartedWalking.AddUObject(this, &USelectAndGenReachablesAction::_HandleUnitStartedWalking);
		
		pUnit->OnUnitFinishedWalking.RemoveAll(this);
		pUnit->OnUnitFinishedWalking.AddUObject(this, &USelectAndGenReachablesAction::_HandleUnitFinishedWalking);
		
		start = pUnit->GetIndex();
		target = FIntPoint(INVALID_POINT_VALUE);
		options.bIsDiagonalIncluded = pUnit->GetUnitData().Stats.bCanDiagonalMove;
		options.ValidTileTypeMask = pUnit->GetUnitData().Stats.ValidTileTypeFlags;
		options.DelayBetweenIterations = 0.0f;
		options.MaxMsPerFrame = 0.0f;
		options.bIsReturnReachableTiles = true;
		options.MaxPathLength = pUnit->GetUnitData().Stats.CurrentMovePoint;
	}
	else
	{		
		start = m_PlayerActions->GetSelectedTileIndex();
		target = FIntPoint(INVALID_POINT_VALUE);		
		options.ValidTileTypeMask = static_cast<uint8>(ETileType::Normal)
									| static_cast<uint8>(ETileType::DoubleCost)
									| static_cast<uint8>(ETileType::TripleCost);
		if (m_bCanUseFlyingOnly)
		{
			options.ValidTileTypeMask  = options.ValidTileTypeMask  | static_cast<uint8>(ETileType::FlyingUnitsOnly);
		}

		options.bIsDiagonalIncluded = m_bCanUseDiagonals;
		options.DelayBetweenIterations = m_DelayBetweenIterations;
		options.MaxMsPerFrame = m_MaxMs;
		options.bIsReturnReachableTiles = true;
		options.MaxPathLength = m_MaxPathLength;
	}
	
	TArray<FIntPoint> neighborIndices =  pGrid->GetReachablesPathfinding()->FindPathWithNotify(start, target, options);
}

#pragma region Event Handlers
void USelectAndGenReachablesAction::_HandlePathfindingCompleted(const TArray<FIntPoint>& _path)
{
	AGrid* const pGrid =  m_PlayerActions->GetGrid();
	for (const FIntPoint& neighborIndex : _path)
	{
		pGrid->AddStateToTileWithNotify(neighborIndex, ETileStateFlags::Reachable);
	}
}

void USelectAndGenReachablesAction::_HandleUnitStartedWalking(AUnit* const _pUnit)
{
	m_PlayerActions->GetGrid()->ClearStateFromTiles(ETileStateFlags::Reachable);
}

void USelectAndGenReachablesAction::_HandleUnitFinishedWalking(AUnit* const _pUnit)
{
	_GenerateReachable();
}

#pragma endregion

