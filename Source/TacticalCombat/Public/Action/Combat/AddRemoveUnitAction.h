// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action/AbstractAction.h"
#include "AddRemoveUnitAction.generated.h"

enum class ETacticalUnitType : uint8;
/**
 * 
 */
UCLASS()
class TACTICALCOMBAT_API UAddRemoveUnitAction : public UAbstractAction
{
	GENERATED_BODY()

public:
	virtual void Execute(const FIntPoint& _index) override;

#pragma region Setter
	FORCEINLINE void SetIsAddingUnit(bool _isAddingUnit) { m_bIsAddingUnit = _isAddingUnit; }
	FORCEINLINE void SetUnitType(ETacticalUnitType _unitType) { m_UnitType = _unitType; }
#pragma endregion

protected:
	bool m_bIsAddingUnit;
	ETacticalUnitType m_UnitType;
};
