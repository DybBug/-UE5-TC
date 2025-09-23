// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EGridShape : uint8
{
	None UMETA(DisplayName = "None"),
	Square UMETA(DisplayName = "Square"),
	Hexagon UMETA(DisplayName = "Hexagon"),
	Triangle UMETA(DisplayName = "Triangle"),
};

UENUM(BlueprintType)
enum class ETileType: uint8
{
	None UMETA(DisplayName = "None"),
	Normal UMETA(DisplayName = "Normal"),
	Obstacle UMETA(DisplayName = "Obstacle"),
};