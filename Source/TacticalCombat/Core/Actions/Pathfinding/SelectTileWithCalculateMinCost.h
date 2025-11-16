// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TacticalCombat//Core/Actions/AbstractAction.h"
#include "TacticalCombat/Core/Actions/Grid/SelectTileAction.h"
#include "SelectTileWithCalculateMinCost.generated.h"

/**
 * 
 */
UCLASS()
class TACTICALCOMBAT_API USelectTileWithCalculateMinCost : public USelectTileAction
{
	GENERATED_BODY()

public:
	virtual void Execute(const FIntPoint& _index) override;

#pragma region Setter
	FORCEINLINE void SetUseDiagonals(bool _bIsDiagonalIncluded) { m_bIsDiagonalIncluded = _bIsDiagonalIncluded; } 
#pragma endregion

protected:
#pragma region Properties
	UPROPERTY(EditAnywhere, Category = "Property", Meta = (DisplayName = "Is Diagonal Included"))
	bool m_bIsDiagonalIncluded;
#pragma endregion
};
