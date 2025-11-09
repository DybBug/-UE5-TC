// Fill out your copyright notice in the Description page of Project Settings.


#include "GridPathfinding.h"
#include "Grid.h"
#include "TacticalCombat/Structure/GridDatas.h"
#include "TacticalCombat/Libraries/UtilityLibrary.h"


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

// Called every frame
void AGridPathfinding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

TArray<FPathfindingNode> AGridPathfinding::GetValidTileNeighborNodes(const FIntPoint& _index, bool _bIsDiagonalIncluded)
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
		if (pTempNeighborTileData && AGrid::IsWalkableTile(pTempNeighborTileData->Type))
		{
			float inputTileHeight = pInputTileData->Transform.GetLocation().Z;
			float tempNeighborTileHeight = pTempNeighborTileData->Transform.GetLocation().Z;
			if (FMath::Abs(inputTileHeight - tempNeighborTileHeight) <= m_Grid->GetTileSize().Z)
			{
				FPathfindingNode neighborNode;
				neighborNode.Index = tempNeighborIndex;
				neighborNode.PreviousIndex = _index;
				
				neighborNodes.Add(neighborNode);
			}
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

TArray<FIntPoint> AGridPathfinding::FindPath(const FIntPoint& _start, const FIntPoint& _target, bool _bIsDiagonalIncluded)
{
	TArray<FIntPoint> path;
	m_StartIndex = _start;
	m_TargetIndex = _target;
	m_bIsDiagonalIncluded = _bIsDiagonalIncluded;

	ClearGeneratedData();

	if (IsInputDataValid())
	{
		FPathfindingNode node;
		node.Index = m_StartIndex;
		node.CostToEnterTile = 1;
		node.CostFromStart = 0;
		node.MinimumCostToTarget = GetMinimumCostBetweenTwoNodes(m_StartIndex, m_TargetIndex, m_bIsDiagonalIncluded);
		DiscoverNode(node);

		while (m_DiscoveredNodeIndices.Num() > 0)		
		{
			if (AnalyseNextDiscoveredNode())
			{
				path = GeneratePath();
				return path;
			}
		}			
	}
	return path;
}

bool AGridPathfinding::IsInputDataValid()
{
	if (m_StartIndex == m_TargetIndex)			return false;
	if (!m_Grid->IsWalkableTile(m_StartIndex))	return false;
	if (!m_Grid->IsWalkableTile(m_TargetIndex)) return false;
	return true;
}

void AGridPathfinding::DiscoverNode(const FPathfindingNode& _node)
{
	m_PathfindingNodesByIndex.Add(_node.Index, _node);
	InsertNodeInDiscoveredArray(_node);
}

int32 AGridPathfinding::GetMinimumCostBetweenTwoNodes(const FIntPoint& _index1, const FIntPoint& _index2, bool _bIsDiagonalIncluded)
{
	if (_bIsDiagonalIncluded)
	{
		int32 x = FMath::Abs(_index1.X - _index2.X);
		int32 y = FMath::Abs(_index1.Y - _index2.Y);
		return FMath::Max(x, y);
	}
	else
	{
		int32 x = FMath::Abs(_index1.X - _index2.X);
		int32 y = FMath::Abs(_index1.Y - _index2.Y);
		return x + y;  
	}
}

bool AGridPathfinding::AnalyseNextDiscoveredNode()
{
	m_CurrentDiscoveredNode = PullCheapestNodeOutOfDiscoveredList();
	m_CurrentNeighborNodes = GetValidTileNeighborNodes(m_CurrentDiscoveredNode.Index, m_bIsDiagonalIncluded);

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
	for (const FIntPoint& invertedPathIndex: invertedPath)
	{
		path.Add(invertedPathIndex);
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
	int32 indexInDiscovered = m_DiscoveredNodeIndices.Find(m_CurrentNeighborNode.Index);
	if (indexInDiscovered == -1)
	{
		FPathfindingNode node;
		node.Index = m_CurrentNeighborNode.Index;
		node.CostToEnterTile = m_CurrentNeighborNode.CostToEnterTile;
		node.CostFromStart = costFromStart;
		node.MinimumCostToTarget = GetMinimumCostBetweenTwoNodes(m_CurrentNeighborNode.Index, m_TargetIndex, m_bIsDiagonalIncluded);
		node.PreviousIndex = m_CurrentDiscoveredNode.Index;
		DiscoverNode(node);

		return (m_CurrentNeighborNode.Index == m_TargetIndex); 
	}

	m_CurrentNeighborNode = *m_PathfindingNodesByIndex.Find(m_CurrentNeighborNode.Index);
	if (costFromStart < m_CurrentNeighborNode.CostFromStart)
	{
		m_DiscoveredNodeIndices.RemoveAt(indexInDiscovered);
		m_DiscoveredNodeSortingCosts.Add(indexInDiscovered);
		
		FPathfindingNode node;
		node.Index = m_CurrentNeighborNode.Index;
		node.CostToEnterTile = m_CurrentNeighborNode.CostToEnterTile;
		node.CostFromStart = costFromStart;
		node.MinimumCostToTarget = GetMinimumCostBetweenTwoNodes(m_CurrentNeighborNode.Index, m_TargetIndex, m_bIsDiagonalIncluded);
		node.PreviousIndex = m_CurrentDiscoveredNode.Index;
		DiscoverNode(node);

		return (m_CurrentNeighborNode.Index == m_TargetIndex); 
	}

	return false;
}

void AGridPathfinding::InsertNodeInDiscoveredArray(const FPathfindingNode& _node)
{
	int32 sortingCost = _node.CostFromStart + _node.MinimumCostToTarget;

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

void AGridPathfinding::ClearGeneratedData()
{
	m_PathfindingNodesByIndex.Empty();
	m_DiscoveredNodeSortingCosts.Empty();
	m_DiscoveredNodeIndices.Empty();
	m_AnalysedNodeIndices.Empty();
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
	neighbors.Add(_index + FIntPoint(+1, -1));
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



