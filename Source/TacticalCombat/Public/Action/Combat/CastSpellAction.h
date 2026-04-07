// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action/AbstractAction.h"
#include "CastSpellAction.generated.h"

enum class ESpellType: uint8;
/**
 * 
 */
UCLASS()
class TACTICALCOMBAT_API UCastSpellAction : public UAbstractAction
{
	GENERATED_BODY()

public:
	virtual void Execute(const FIntPoint& _index) override;

	void SetSpellType(ESpellType _type) { m_SpellType = _type; }


protected:
	ESpellType m_SpellType;
};
