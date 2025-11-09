// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../AbstractAction.h"
#include "TacticalCombat/Core/PlayerActions.h"
#include "SetTileTypeAction.generated.h"

enum class ETileType :uint8;
/**
 * 
 */
UCLASS()
class TACTICALCOMBAT_API USetTileTypeAction : public UAbstractAction
{
	GENERATED_BODY()

public:
	virtual void Execute(const FIntPoint& _index) override;

#pragma region Getter
	FORCEINLINE ETileType GetTileType() const { return m_TileType; }
#pragma endregion

#pragma region Setter
	FORCEINLINE void SetTileType(ETileType _value) { m_TileType = _value; }
#pragma endregion
protected:
	ETileType m_TileType;
};
