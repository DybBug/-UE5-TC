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
	FORCEINLINE void SetCanUseDiagonals(bool _bCanUseDiagonals) { m_bCanUseDiagonals = _bCanUseDiagonals; }
	FORCEINLINE void SetCanUseFlyingOnly(bool _bCanUseFlyingOnly) { m_bCanUseFlyingOnly = _bCanUseFlyingOnly; }
	FORCEINLINE void SetDelayBetweenIterations(float _value) {m_DelayBetweenIterations = _value; }
	FORCEINLINE void SetMaxMs(float _value) {m_MaxMs = _value; }
#pragma endregion

protected:
	bool m_bCanUseDiagonals;
	bool m_bCanUseFlyingOnly;
	float m_DelayBetweenIterations;
	float m_MaxMs;
private:
	UFUNCTION()
	void _OnPathfindingCompleted(const TArray<FIntPoint>& _path);
	
};
