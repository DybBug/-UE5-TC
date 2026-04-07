// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action/Grid/SelectTileAndUnitAction.h"
#include "Combat/Spells/Types/SpellCast.h"
#include "ShowSpellRangePatternAction.generated.h"

/**
 * 
 */
UCLASS()
class TACTICALCOMBAT_API UShowSpellRangePatternAction : public USelectTileAndUnitAction
{
	GENERATED_BODY()

public:
	virtual void Execute(const FIntPoint& _index) override;

public:
	void ShowSpellRangePattern();

public :
#pragma region Setter
	FORCEINLINE void SetCast(const FSpellCast& _value) { m_Cast = _value;}
#pragma endregion
	
protected:
#pragma region Properties
	FSpellCast m_Cast;
#pragma endregion

};
