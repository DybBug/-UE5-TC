// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PathfindingDatas.generated.h"

USTRUCT(BlueprintType)
struct FPathfindingNode
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntPoint Index = FIntPoint(-999, -999);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CostToEnterTile = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CostFromStart = 999999;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MinimumCostToTarget = 999999;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntPoint PreviousIndex = FIntPoint(-999, -999);
};