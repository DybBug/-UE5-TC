// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action/Pathfinding/FindPathToTargetAction.h"
#include "MoveUnitOnGridAction.generated.h"

/**
 * 
 */
UCLASS()
class TACTICALCOMBAT_API UMoveUnitOnGridAction : public USetTileTypeAction
{
	GENERATED_BODY()

public:
	virtual void Execute(const FIntPoint& _index) override;

#pragma region Setter
	FORCEINLINE void SetCanUseDiagonals(bool _bCanUseDiagonals) { m_bCanUseDiagonals = _bCanUseDiagonals; }
	FORCEINLINE void SetCanUseFlyingOnly(bool _bCanUseFlyingOnly) { m_bCanUseFlyingOnly = _bCanUseFlyingOnly; }
	FORCEINLINE void SetDelayBetweenIterations(float _value) {m_DelayBetweenIterations = _value; }
	FORCEINLINE void SetMaxMs(float _value) {m_MaxMs = _value; }
	FORCEINLINE void SetMoveDurationPerTile(float _value) { m_MoveDurationPerTile = _value; }
#pragma endregion

protected:
	bool m_bCanUseDiagonals;
	bool m_bCanUseFlyingOnly;
	float m_DelayBetweenIterations;
	float m_MaxMs;
	float m_MoveDurationPerTile;

	TWeakObjectPtr<AUnit> m_CurrentUnit;
private:
	UFUNCTION()
	void _HandlePathfindingCompleted(const TArray<FIntPoint>& _path);

	UFUNCTION()
	void _HandleUnitFinishedWalking(AUnit* const _pUnit);
};

