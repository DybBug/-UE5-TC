// Fill out your copyright notice in the Description page of Project Settings.


#include "Action/Pathfinding/MoveUnitOnGridAction.h"
#include "Core/PlayerActions.h"
#include "Grid/Grid.h"
#include "Grid/GridPathfinding.h"
#include "Unit/Unit.h"

void UMoveUnitOnGridAction::Execute(const FIntPoint& _index)
{
	if (!m_PlayerActions.IsValid()) return;
	
	AGrid* const pGrid =  m_PlayerActions->GetGrid();
	pGrid->ClearStateFromTiles(ETileStateFlags::InPath);

	m_CurrentUnit = m_PlayerActions->GetSelectedUnit();
    if (!m_CurrentUnit.IsValid()) return;

	m_CurrentUnit->OnUnitFinishedWalking.RemoveAll(this);
	m_CurrentUnit->OnUnitFinishedWalking.AddUObject(this, &UMoveUnitOnGridAction::_HandleUnitFinishedWalking);

	pGrid->GetGridPathfinding()->OnPathfindingCompleted.RemoveAll(this);
	pGrid->GetGridPathfinding()->OnPathfindingCompleted.AddUObject(this, &UMoveUnitOnGridAction::_HandlePathfindingCompleted);

	FIntPoint start = m_PlayerActions->GetSelectedTileIndex();
	FIntPoint target = _index;

	const FUnitTableRow& unitData = m_CurrentUnit->GetUnitData();
	m_bCanUseDiagonals = unitData.Stats.bCanDiagonalMove;
	FPathFindingOptions options = {
		.bIsDiagonalIncluded = m_bCanUseDiagonals,
		.ValidTileTypeMask = unitData.Stats.ValidTileTypeFlags,
		.DelayBetweenIterations = m_DelayBetweenIterations,
		.MaxMsPerFrame = m_MaxMs,
		.bIsReturnReachableTiles = false,
		.MaxPathLength = unitData.Stats.CurrentMovePoint
	};
	TArray<FIntPoint> neighborIndices =  pGrid->GetGridPathfinding()->FindPathWithNotify(start, target, options);
	
}

void UMoveUnitOnGridAction::_HandlePathfindingCompleted(const TArray<FIntPoint>& _path)
{
	if (!m_PlayerActions.IsValid()) return;
	if (!m_CurrentUnit.IsValid()) return;

	if (m_PlayerActions->GetSelectedUnit() == m_CurrentUnit)
	{	
		AGrid* const pGrid =  m_PlayerActions->GetGrid();
		for (const FIntPoint& neighborIndex : _path)
		{
			pGrid->AddStateToTileWithNotify(neighborIndex, ETileStateFlags::InPath);
		}

		m_CurrentUnit->SetMoveDurationPerTile(m_MoveDurationPerTile);
		m_CurrentUnit->FollowPathWithNotify(_path);
	}
}

void UMoveUnitOnGridAction::_HandleUnitFinishedWalking(AUnit* const _pUnit)
{
	if (_pUnit != m_CurrentUnit) return;

	m_PlayerActions->GetGrid()->ClearStateFromTiles(ETileStateFlags::InPath);
}

