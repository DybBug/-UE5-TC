// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TacticalCombat/Misc/Enums.h"
#include "TacticalCombat/Structure/PathfindingDatas.h"
#include "GridPathfinding.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPathfindingNodeUpdated, const FIntPoint&);
DECLARE_MULTICAST_DELEGATE(FOnPathfindingNodeCleared);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPathfindingCompleted, const TArray<FIntPoint>&);

class AGrid;

UCLASS()
class TACTICALCOMBAT_API AGridPathfinding : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridPathfinding();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	TArray<FPathfindingNode> GetValidTileNeighborNodes(const FIntPoint& _index, bool _bIsDiagonalIncluded = false);
	TArray<FIntPoint> GetNeighborIndices(const FIntPoint& _index, bool _bIsDiagonalIncluded = false);

	TArray<FIntPoint> FindPath(const FIntPoint& _start, const FIntPoint& _target, bool _bIsDiagonalIncluded, float _delayTime, float _maxMs);
	bool IsInputDataValid();
	void DiscoverNode(const FPathfindingNode& _node);
	int32 GetMinimumCostBetweenTwoNodes(const FIntPoint& _index1, const FIntPoint& _index2, bool _bIsDiagonalIncluded);
	bool AnalyseNextDiscoveredNode();
	TArray<FIntPoint> GeneratePath();
	FPathfindingNode PullCheapestNodeOutOfDiscoveredList();
	bool DiscoverNextNeighbor();
	void InsertNodeInDiscoveredArray(const FPathfindingNode& _node);
	void ClearGeneratedData();
	bool IsDiagonal(const FIntPoint& _index1, const FIntPoint& _index2);

	UFUNCTION()
	void FindPathWithDelay();

#pragma region Getter
	FORCEINLINE const FPathfindingNode* FindPathFindingNode(const FIntPoint& _index) const { return m_PathfindingNodesByIndex.Find(_index); }
	FORCEINLINE int32 FindDiscoveredNodeIndex(const FIntPoint& _index) const { return m_DiscoveredNodeIndices.Find(_index); }
	FORCEINLINE int32 FindDiscoveredNodeSortingCost(uint32 _index) const  { return m_DiscoveredNodeSortingCosts.IsValidIndex(_index) ? m_DiscoveredNodeSortingCosts[_index] : INVALID_SORTING_COST;}
	FORCEINLINE int32 FindAnalysedNodeIndex(const FIntPoint& _index) const {return m_AnalysedNodeIndices.Find(_index); }
#pragma endregion 
	
#pragma region Setter
	FORCEINLINE void SetGrid(AGrid* const _pGrid) { m_Grid = _pGrid; }
#pragma endregion

#pragma region Events
	FOnPathfindingNodeUpdated OnPathfindingNodeUpdated;
	FOnPathfindingNodeCleared OnPathfindingNodeCleared;
	FOnPathfindingCompleted OnPathfindingCompleted;
#pragma endregion

private:
#pragma region Internals
	UPROPERTY(VisibleInstanceOnly, Category = "Internal", Meta = (DisplayName = "Grid"))
	TWeakObjectPtr<AGrid> m_Grid;

	UPROPERTY(VisibleInstanceOnly, Category = "Internal", Meta = (DisplayName = "Start Index"))
	FIntPoint m_StartIndex;

	UPROPERTY(VisibleInstanceOnly, Category = "Internal", Meta = (DisplayName = "End Index"))
	FIntPoint m_TargetIndex;

	UPROPERTY(VisibleInstanceOnly, Category = "Internal", Meta = (DisplayName = "Is Diagonal Included"))
	bool m_bIsDiagonalIncluded;

	UPROPERTY(VisibleInstanceOnly, Category = "Internal", Meta = (DisplayName = "Discovered Node Indices"))
	TArray<FIntPoint> m_DiscoveredNodeIndices;
	
	UPROPERTY(VisibleInstanceOnly, Category = "Internal", Meta = (DisplayName = "Analysed Node Indices"))
	TArray<FIntPoint> m_AnalysedNodeIndices;

	UPROPERTY(VisibleInstanceOnly, Category = "Internal", Meta = (DisplayName = "Current Neighbor Nodes"))
	TArray<FPathfindingNode> m_CurrentNeighborNodes;

	UPROPERTY(VisibleAnywhere, Category = "Internal", Meta = (DisplayName = "Pathfinding Nodes By Index"))
	TMap<FIntPoint, FPathfindingNode> m_PathfindingNodesByIndex;

	UPROPERTY(VisibleAnywhere, Category = "Internal", Meta = (DisplayName = "Discovered Node Sorting Costs"))
	TArray<int32> m_DiscoveredNodeSortingCosts;

	UPROPERTY(VisibleInstanceOnly, Category = "Internal", Meta = (DisplayName = "Current Discovered Node"))
    FPathfindingNode m_CurrentDiscoveredNode;

	UPROPERTY(VisibleInstanceOnly, Category = "Internal", Meta = (DisplayName = "Current Neighbor Node"))
	FPathfindingNode m_CurrentNeighborNode;

	UPROPERTY(VisibleInstanceOnly, Category = "Internal", Meta = (DisplayName = "Delay Between Iterations"))
	float m_DelayBetweenIterations;

	UPROPERTY(VisibleInstanceOnly, Category = "Internal", Meta = (DisplayName ="Max Ms Per Frame"))
	float m_MaxMsPerFrame;

	UPROPERTY(VisibleInstanceOnly, Category = "Internal", Meta = (DisplayName ="Max Ms Per Frame"))
	FDateTime m_LoopStartDateTime;

	UPROPERTY()
	FLatentActionInfo m_LatentInfo_FindPathWithDelay;

#pragma endregion

private:
	TArray<FIntPoint> _GetNeighborIndicesForSquare(const FIntPoint& _index, bool _bIsDiagonalIncluded);
	TArray<FIntPoint> _GetNeighborIndicesForHexagon(const FIntPoint& _index);
	TArray<FIntPoint> _GetNeighborIndicesForTriangle(const FIntPoint& _index, bool _bIsDiagonalIncluded);

	int32 _GetTileSortingCost(const FPathfindingNode& _node);	
};


