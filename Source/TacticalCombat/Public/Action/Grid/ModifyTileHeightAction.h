// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action/AbstractAction.h"
#include "ModifyTileHeightAction.generated.h"

/**
 * 
 */
UCLASS()
class TACTICALCOMBAT_API UModifyTileHeightAction : public UAbstractAction
{
	GENERATED_BODY()

public:
	virtual void Execute(const FIntPoint& _index) override;

	FORCEINLINE void SetIsIncrease(bool _isIncrease) { m_bIsIncrease = _isIncrease; }
	FORCEINLINE bool IsIncrease() const { return m_bIsIncrease; }
protected:
	bool m_bIsIncrease;
	
};
