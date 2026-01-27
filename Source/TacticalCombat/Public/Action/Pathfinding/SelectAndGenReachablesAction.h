// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action/Grid/SelectTileAndUnitAction.h"
#include "SelectAndGenReachablesAction.generated.h"

/**
 * 
 */
UCLASS()
class TACTICALCOMBAT_API USelectAndGenReachablesAction : public USelectTileAndUnitAction
{
	GENERATED_BODY()

public:
	virtual void Execute(const FIntPoint& _index) override;

#pragma region Setter
	FORCEINLINE void SetCanUseDiagonals(bool _value) { m_bCanUseDiagonals = _value; }
	FORCEINLINE void SetCanUseFlyingOnly(bool _value) { m_bCanUseFlyingOnly = _value; }
	FORCEINLINE void SetDelayBetweenIterations(float _value) { m_DelayBetweenIterations = _value; }
	FORCEINLINE void SetMaxMs(float _value) { m_MaxMs = _value; }
	FORCEINLINE void SetMaxPathLength(int32 _value) { m_MaxPathLength = _value; }	
#pragma endregion

protected:
	bool m_bCanUseDiagonals;
	bool m_bCanUseFlyingOnly;
	float m_DelayBetweenIterations;
	float m_MaxMs;
	int32 m_MaxPathLength;

private:
	void _GenerateReachable();
	
#pragma region Event Handlers
	UFUNCTION()
	void _HandlePathfindingCompleted(const TArray<FIntPoint>& _path);

	UFUNCTION()
	void _HandleUnitStartedWalking(AUnit* const _pUnit);

	UFUNCTION()
	void _HandleUnitFinishedWalking(AUnit* const _pUnit);
#pragma endregion 

};