// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "Shared/SharedConstants.h"
#include "PathfindingNode.generated.h"

USTRUCT(BlueprintType)
struct FPathfindingNode
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntPoint Index = FIntPoint(Grid::INVALID_POINT_VALUE, Grid::INVALID_POINT_VALUE);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CostToEnterTile = GridPathfinding::DEFAULT_COST_TO_ENTER;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CostFromStart = GridPathfinding::DEFAULT_COST;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MinimumCostToTarget = GridPathfinding::DEFAULT_COST;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntPoint PreviousIndex = FIntPoint(Grid::INVALID_POINT_VALUE, Grid::INVALID_POINT_VALUE);
};


