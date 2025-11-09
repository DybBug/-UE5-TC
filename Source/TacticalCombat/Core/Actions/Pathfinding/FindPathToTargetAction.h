// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TacticalCombat/Core/Actions/Grid/SetTileTypeAction.h"
#include "FindPathToTargetAction.generated.h"

/**
 * 
 */
UCLASS()
class TACTICALCOMBAT_API UFindPathToTargetAction : public USetTileTypeAction
{
	GENERATED_BODY()

public:
	virtual void Execute(const FIntPoint& _index) override;

#pragma region Setter
	FORCEINLINE void SetUseDiagonals(bool _bUseDiagonals) {m_bUseDiagonals = _bUseDiagonals; }
#pragma endregion

protected:
	bool m_bUseDiagonals;
	
};
