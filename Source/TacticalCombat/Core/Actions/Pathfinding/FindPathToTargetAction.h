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
	FORCEINLINE void SetDelayBetweenIterations(float _value) {m_DelayBetweenIterations = _value; }
	FORCEINLINE void SetMaxMs(float _value) {m_MaxMs = _value; }
#pragma endregion

protected:
	bool m_bUseDiagonals;
	float m_DelayBetweenIterations;
	float m_MaxMs;
private:
	UFUNCTION()
	void _OnPathfindingCompleted(const TArray<FIntPoint>& _path);
	
};
