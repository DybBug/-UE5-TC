// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "TacticalCombat/Misc/Enums.h"
#include "PathfindingDatas.generated.h"

USTRUCT(BlueprintType)
struct FPathfindingNode
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntPoint Index = FIntPoint(INVALID_POINT_VALUE, INVALID_POINT_VALUE);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CostToEnterTile = DEFAULT_COST_TO_ENTER;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CostFromStart = DEFAULT_COST;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MinimumCostToTarget = DEFAULT_COST;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntPoint PreviousIndex = FIntPoint(INVALID_POINT_VALUE, INVALID_POINT_VALUE);
};


