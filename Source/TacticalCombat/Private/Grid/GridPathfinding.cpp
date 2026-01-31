// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid/GridPathfinding.h"
#include "Grid/Grid.h"
#include "Grid/Types/PathfindingNode.h"
#include "Library/UtilityLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

static TMap<ETileType, int> s_TileTypeToCost =
{
	{ETileType::None, 0},
	{ETileType::Normal, 1},
	{ETileType::Obstacle, 0},
	{ETileType::DoubleCost, 2},
	{ETileType::TripleCost, 3},
	{ETileType::FlyingUnitsOnly, 1}
};

// Sets default values
AGridPathfinding::AGridPathfinding()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AGridPathfinding::BeginPlay()
{
	Super::BeginPlay();
	
}

TArray<FPathfindingNode> AGridPathfinding::GetValidTileNeighborNodes(const FIntPoint& _index, bool _bIsDiagonalIncluded, uint8 _validTileTypeMask)
{
	TArray<FPathfindingNode> neighborNodes;
	const FTileData* pInputTileData = m_Grid->GetGridTileMap().Find(_index);
	if (!pInputTileData)
	{
		return neighborNodes;
	}
	 
	TArray<FIntPoint> tempNeighborIndices = GetNeighborIndices(_index, _bIsDiagonalIncluded);
	for (const FIntPoint& tempNeighborIndex : tempNeighborIndices)
	{
		const FTileData* pTempNeighborTileData = m_Grid->GetGridTileMap().Find(tempNeighborIndex);
		if (!pTempNeighborTileData) continue;
		if (pTempNeighborTileData->UnitOnTile.IsValid()) continue;
		if ((_validTileTypeMask & static_cast<uint8>(pTempNeighborTileData->Type)) == 0) continue;

		float inputTileHeight = pInputTileData->Transform.GetLocation().Z;
		float tempNeighborTileHeight = pTempNeighborTileData->Transform.GetLocation().Z;
		if (FMath::Abs(inputTileHeight - tempNeighborTileHeight) <= m_Grid->GetTileSize().Z)
		{
			FPathfindingNode neighborNode;
			neighborNode.Index = tempNeighborIndex;
			neighborNode.PreviousIndex = _index;
			neighborNode.CostToEnterTile = s_TileTypeToCost[pTempNeighborTileData->Type];
			
			neighborNodes.Add(neighborNode);
			
		}
	}
	return neighborNodes;
}

TArray<FIntPoint> AGridPathfinding::GetNeighborIndices(const FIntPoint& _index, bool _bIsDiagonalIncluded)
{
	switch (m_Grid->GetGridShape())
	{
		case EGridShape::None:		return TArray<FIntPoint>();
		case EGridShape::Square:	return _GetNeighborIndicesForSquare(_index, _bIsDiagonalIncluded);
		case EGridShape::Hexagon:	return _GetNeighborIndicesForHexagon(_index);
		case EGridShape::Triangle:	return _GetNeighborIndicesForTriangle(_index, _bIsDiagonalIncluded);
		default:					return TArray<FIntPoint>();
	}
}

TArray<FIntPoint> AGridPathfinding::FindPathWithNotify(const FIntPoint& _start, const FIntPoint& _target, const FPathFindingOptions& _options)
{
	TArray<FIntPoint> path;
	
	m_StartIndex = _start;
	m_TargetIndex = _target;
	
	m_PathFindingOptions = _options;

	FLatentActionManager& LatentActionManager = GetWorld()->GetLatentActionManager();
	LatentActionManager.RemoveActionsForObject(m_LatentInfo_FindPathWithDelay.CallbackTarget);
	m_LatentInfo_FindPathWithDelay.CallbackTarget = nullptr;
	
	ClearGeneratedDataWithNotify();

	if (IsInputDataValid())
	{
		FPathfindingNode node;
		node.Index = m_StartIndex;
		node.CostToEnterTile = 1;
		node.CostFromStart = 0;
		node.MinimumCostToTarget = GetMinimumCostBetweenTwoNodes(m_StartIndex, m_TargetIndex, m_PathFindingOptions.bIsDiagonalIncluded);
		DiscoverNodeWithNotify(node);

		if (m_PathFindingOptions.DelayBetweenIterations > 0.0f)
		{
			FindPathWithDelayWithNotify();
			return path;
		}
		
		while (m_DiscoveredNodeIndices.Num() > 0)		
		{
			if (AnalyseNextDiscoveredNodeWithNotify())
			{
				path = GeneratePath();
				if (OnPathfindingCompleted.IsBound())
				{
					OnPathfindingCompleted.Broadcast(path);
				}
				return path;
			}
		}				
	}
	
	if (m_PathFindingOptions.bIsReturnReachableTiles)
	{
		path = m_AnalysedNodeIndices;
	}
	
	if (OnPathfindingCompleted.IsBound())
	{
		OnPathfindingCompleted.Broadcast(path);
	}
	return path;
}

bool AGridPathfinding::IsInputDataValid()
{
	if (m_StartIndex == m_TargetIndex) return false;
	if (!m_Grid->IsWalkableTile(m_StartIndex))	return false;
	if (!m_PathFindingOptions.bIsReturnReachableTiles)
	{		
		
		if (!m_Grid->IsWalkableTile(m_TargetIndex)) return false;
		if (GetMinimumCostBetweenTwoNodes(m_StartIndex, m_TargetIndex, m_PathFindingOptions.bIsDiagonalIncluded) > m_PathFindingOptions.MaxPathLength)
			return false;

		const FTileData* pTile = m_Grid->GetGridTileMap().Find(m_TargetIndex);
		if (pTile == nullptr) return false;
		if (pTile->UnitOnTile.IsValid()) return false;
	}
	
	return true;
}

void AGridPathfinding::DiscoverNodeWithNotify(const FPathfindingNode& _node)
{
	m_PathfindingNodesByIndex.Add(_node.Index, _node);
	InsertNodeInDiscoveredArray(_node);
	if (OnPathfindingNodeUpdated.IsBound())
	{
		OnPathfindingNodeUpdated.Broadcast(_node.Index);
	}
}

int32 AGridPathfinding::GetMinimumCostBetweenTwoNodes(const FIntPoint& _index1, const FIntPoint& _index2, bool _bIsDiagonalIncluded)
{
	switch (m_Grid->GetGridShape())
	{
		case EGridShape::None:
		{
			return -1;
		}
		case EGridShape::Square:
		{
			if (_bIsDiagonalIncluded)
			{
				int32 dx = FMath::Abs(_index1.X - _index2.X);
				int32 dy = FMath::Abs(_index1.Y - _index2.Y);
				return FMath::Max(dx, dy);
			}
			else
			{
				int32 dx = FMath::Abs(_index1.X - _index2.X);
				int32 dy = FMath::Abs(_index1.Y - _index2.Y);
				return dx + dy;				
			}
		}
		case EGridShape::Hexagon:
		{
			int32 dx = FMath::Abs(_index1.X - _index2.X);
			int32 dy = FMath::Max((FMath::Abs(_index1.Y - _index2.Y) - dx) * 0.5f, 0);
			return dx + dy;
		}
		case EGridShape::Triangle:
		{
			if (_bIsDiagonalIncluded)
			{
				int32 dx = FMath::Abs(_index1.X - _index2.X);
				int32 dy = FMath::Abs(_index1.Y - _index2.Y);
				return (dx + dy) * 0.5f;		
			}
			else
			{
				int32 dx = FMath::Abs(_index1.X - _index2.X);
				int32 dy = FMath::Abs(_index1.Y - _index2.Y);

				bool isStartingTileFacingUp = UUtilityLibrary::IsIntEven(_index1.X) == UUtilityLibrary::IsIntEven(_index1.Y);
		
				bool isPartOfTheNormalZone = isStartingTileFacingUp
											? (_index2.X < _index1.X) ? ((dx - 1) <= dy) : (dx <= dy)
											: (_index2.X < _index1.X) ? (dx <= dy) : ((dx - 1) <= dy);

				if (isPartOfTheNormalZone)
				{
					return dx + dy;
				}
				else
				{
					bool isTileFacingUp = UUtilityLibrary::IsIntEven(dx) == UUtilityLibrary::IsIntEven(dy);
					if (isTileFacingUp)
					{
						return dx * 2;
					}
					else
					{						
						bool isBellowStartingTile = _index2.X < _index1.X;
						int32 offset = isBellowStartingTile ? -1 : 1;
						return (dx * 2) + (isStartingTileFacingUp ? offset : -offset);			
					}
				}				
			}
		}
		default:
		{
			checkf(false, TEXT("GridShape value %d is invalid in %s"), 
				   static_cast<int>(m_Grid->GetGridShape()), 
				   TEXT(__FUNCTION__));
			return -1;	
		}
	}

}

bool AGridPathfinding::AnalyseNextDiscoveredNodeWithNotify()
{
	m_CurrentDiscoveredNode = PullCheapestNodeOutOfDiscoveredList();

	if (OnPathfindingNodeUpdated.IsBound())
	{
		OnPathfindingNodeUpdated.Broadcast(m_CurrentDiscoveredNode.Index);
	}
	
	m_CurrentNeighborNodes = GetValidTileNeighborNodes(m_CurrentDiscoveredNode.Index, m_PathFindingOptions.bIsDiagonalIncluded, m_PathFindingOptions.ValidTileTypeMask);

	while (m_CurrentNeighborNodes.Num() > 0)
	{
		if (DiscoverNextNeighbor())
		{
			return true;
		}
	}
	return false;
}

TArray<FIntPoint> AGridPathfinding::GeneratePath()
{	
	TArray<FIntPoint> invertedPath;
	FIntPoint currentIndex = m_TargetIndex;
	while (currentIndex != m_StartIndex)
	{
		invertedPath.Add(currentIndex);
		FPathfindingNode* const pCurrentNode = m_PathfindingNodesByIndex.Find(currentIndex);
		currentIndex = pCurrentNode->PreviousIndex;
	}
	
	TArray<FIntPoint> path;
	for (int i = invertedPath.Num() - 1; i >= 0; --i)
	{
		path.Add(invertedPath[i]);
	}

	return path;
}

FPathfindingNode AGridPathfinding::PullCheapestNodeOutOfDiscoveredList()
{
	FIntPoint nodeIndex = m_DiscoveredNodeIndices[0];
	m_DiscoveredNodeSortingCosts.RemoveAt(0);
	m_DiscoveredNodeIndices.RemoveAt(0);
	m_AnalysedNodeIndices.Add(nodeIndex);

	return m_PathfindingNodesByIndex[nodeIndex];
}

bool AGridPathfinding::DiscoverNextNeighbor()
{
	m_CurrentNeighborNode = m_CurrentNeighborNodes[0];
	m_CurrentNeighborNodes.RemoveAt(0);

	if (m_AnalysedNodeIndices.Contains(m_CurrentNeighborNode.Index))
	{
		return false;
	}

	int32 costFromStart = m_CurrentDiscoveredNode.CostFromStart + m_CurrentNeighborNode.CostToEnterTile;
	if (costFromStart > m_PathFindingOptions.MaxPathLength)
	{
		return false;
	}
	
	int32 indexInDiscovered = m_DiscoveredNodeIndices.Find(m_CurrentNeighborNode.Index);
	if (indexInDiscovered == -1)
	{
		FPathfindingNode node;
		node.Index = m_CurrentNeighborNode.Index;
		node.CostToEnterTile = m_CurrentNeighborNode.CostToEnterTile;
		node.CostFromStart = costFromStart;
		node.MinimumCostToTarget = GetMinimumCostBetweenTwoNodes(m_CurrentNeighborNode.Index, m_TargetIndex, m_PathFindingOptions.bIsDiagonalIncluded);
		node.PreviousIndex = m_CurrentDiscoveredNode.Index;
		DiscoverNodeWithNotify(node);

		return (m_CurrentNeighborNode.Index == m_TargetIndex); 
	}

	m_CurrentNeighborNode = *m_PathfindingNodesByIndex.Find(m_CurrentNeighborNode.Index);
	if (costFromStart < m_CurrentNeighborNode.CostFromStart)
	{
		m_DiscoveredNodeIndices.RemoveAt(indexInDiscovered);
		m_DiscoveredNodeSortingCosts.RemoveAt(indexInDiscovered);
		
		FPathfindingNode node;
		node.Index = m_CurrentNeighborNode.Index;
		node.CostToEnterTile = m_CurrentNeighborNode.CostToEnterTile;
		node.CostFromStart = costFromStart;
		node.MinimumCostToTarget = GetMinimumCostBetweenTwoNodes(m_CurrentNeighborNode.Index, m_TargetIndex, m_PathFindingOptions.bIsDiagonalIncluded);
		node.PreviousIndex = m_CurrentDiscoveredNode.Index;
		DiscoverNodeWithNotify(node);

		return (m_CurrentNeighborNode.Index == m_TargetIndex); 
	}

	return false;
}

void AGridPathfinding::InsertNodeInDiscoveredArray(const FPathfindingNode& _node)
{
	int32 sortingCost = _GetTileSortingCost(_node);


	// 1. 비어있을 경우
	if (m_DiscoveredNodeSortingCosts.Num() == 0)
	{
		m_DiscoveredNodeSortingCosts.Add(sortingCost);
		m_DiscoveredNodeIndices.Add(_node.Index);
		return;
	}

	// 2. 가장 큰 경우
	const int32 lastSortingCost = m_DiscoveredNodeSortingCosts.Last();
	if (sortingCost >= lastSortingCost)
	{
		m_DiscoveredNodeSortingCosts.Add(sortingCost);
		m_DiscoveredNodeIndices.Add(_node.Index);
		return;
	}

	for (int32 i = 0; i < m_DiscoveredNodeSortingCosts.Num(); ++i)
	{
		if (sortingCost < m_DiscoveredNodeSortingCosts[i])
		{
			m_DiscoveredNodeSortingCosts.Insert(sortingCost, i);
			m_DiscoveredNodeIndices.Insert(_node.Index, i);
			break;
		}
	}
}

void AGridPathfinding::ClearGeneratedDataWithNotify()
{
	m_PathfindingNodesByIndex.Empty();
	m_DiscoveredNodeSortingCosts.Empty();
	m_DiscoveredNodeIndices.Empty();
	m_AnalysedNodeIndices.Empty();
	
	if (OnPathfindingNodeCleared.IsBound())
	{
		OnPathfindingNodeCleared.Broadcast();
	}
}

bool AGridPathfinding::IsDiagonal(const FIntPoint& _index1, const FIntPoint& _index2)
{
	TArray<FIntPoint> neighborIndices = GetNeighborIndices(_index1);
	return !neighborIndices.Contains(_index2);
}

void AGridPathfinding::FindPathWithDelayWithNotify()
{
	m_LoopStartDateTime = FDateTime::Now();
	
	while (m_DiscoveredNodeIndices.Num() > 0)		
	{
		if (AnalyseNextDiscoveredNodeWithNotify())
		{
			TArray<FIntPoint> path = GeneratePath();
			if (OnPathfindingCompleted.IsBound())
			{
				OnPathfindingCompleted.Broadcast(path);
			}
			return;
		}

		if (m_PathFindingOptions.MaxMsPerFrame > 0.0f)
		{
			float deltaMs = (FDateTime::Now() - m_LoopStartDateTime).GetTotalMilliseconds();
			if (deltaMs < m_PathFindingOptions.MaxMsPerFrame)
			{
				continue;
			}
		}
		
		if (m_LatentInfo_FindPathWithDelay.CallbackTarget == nullptr)
		{		
			m_LatentInfo_FindPathWithDelay.UUID = __LINE__;
			m_LatentInfo_FindPathWithDelay.CallbackTarget = this;
			m_LatentInfo_FindPathWithDelay.ExecutionFunction = "FindPathWithDelay";
			m_LatentInfo_FindPathWithDelay.Linkage = 0;	
		}
		UKismetSystemLibrary::RetriggerableDelay(GetWorld(), m_PathFindingOptions.DelayBetweenIterations, m_LatentInfo_FindPathWithDelay);
		return;
	}
	
	if (OnPathfindingCompleted.IsBound())
	{
		TArray<FIntPoint> path;
		OnPathfindingCompleted.Broadcast(path);
	}
}

TArray<FIntPoint> AGridPathfinding::_GetNeighborIndicesForSquare(const FIntPoint& _index, bool _bIsDiagonalIncluded)
{
	TArray <FIntPoint> neighbors;
	neighbors.Add(_index + FIntPoint(+1, +0)); // top
	neighbors.Add(_index + FIntPoint(+0, +1)); // right
	neighbors.Add(_index + FIntPoint(-1, +0)); // bottom
	neighbors.Add(_index + FIntPoint(+0, -1)); // left
	if (_bIsDiagonalIncluded)
	{
		neighbors.Add(_index + FIntPoint(+1, -1)); // left_top
		neighbors.Add(_index + FIntPoint(+1, +1)); // right_top
		neighbors.Add(_index + FIntPoint(-1, +1)); // right_bottom
		neighbors.Add(_index + FIntPoint(-1, -1)); // left_bottom
	}
	return neighbors;
}

TArray<FIntPoint> AGridPathfinding::_GetNeighborIndicesForHexagon(const FIntPoint& _index)
{
	TArray<FIntPoint> neighbors;
	neighbors.Add(_index + FIntPoint(+1, -1));
	neighbors.Add(_index + FIntPoint(+1, +1));
	neighbors.Add(_index + FIntPoint(+0, +2));
	neighbors.Add(_index + FIntPoint(-1, +1));
	neighbors.Add(_index + FIntPoint(-1, -1));
	neighbors.Add(_index + FIntPoint(+0, -2));
	return neighbors;
}

TArray<FIntPoint> AGridPathfinding::_GetNeighborIndicesForTriangle(const FIntPoint& _index, bool _bIsDiagonalIncluded)
{
	TArray<FIntPoint> neighbors;

	neighbors.Add(_index + FIntPoint(0, -1));
	neighbors.Add(_index + FIntPoint(0, 1));

	bool bIsUpwardTriangle = UUtilityLibrary::IsIntEven(_index.X) == UUtilityLibrary::IsIntEven(_index.Y);
	if (bIsUpwardTriangle)
	{
		neighbors.Add(_index + FIntPoint(-1, 0));
	}
	else
	{
		neighbors.Add(_index + FIntPoint(1, 0));
	}

	if (_bIsDiagonalIncluded)
	{
		if (bIsUpwardTriangle)
		{
			neighbors.Add(_index + FIntPoint(1, 0));
			neighbors.Add(_index + FIntPoint(-1, 2));
			neighbors.Add(_index + FIntPoint(-1, -2));
		}
		else
		{
			neighbors.Add(_index + FIntPoint(-1, 0));
			neighbors.Add(_index + FIntPoint(1, 2));
			neighbors.Add(_index + FIntPoint(1, -2));
		}
	}
	
	return neighbors;
}

int32 AGridPathfinding::_GetTileSortingCost(const FPathfindingNode& _node)
{
	const int32 diagonalCost = IsDiagonal(_node.Index, _node.PreviousIndex) ? 1 : 0;
	return (_node.CostFromStart + _node.MinimumCostToTarget) * 2 + diagonalCost;
}