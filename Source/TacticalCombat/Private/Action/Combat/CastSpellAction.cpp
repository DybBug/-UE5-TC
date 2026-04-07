// Fill out your copyright notice in the Description page of Project Settings.


#include "Action/Combat/CastSpellAction.h"

#include "Combat/CombatSystem.h"
#include "Core/PlayerActions.h"

void UCastSpellAction::Execute(const FIntPoint& _index)
{
	Super::Execute(_index);
	if (m_PlayerActions == nullptr) return;

	ACombatSystem* pCombatSystem = m_PlayerActions->GetCombatSystem();
	check(pCombatSystem);

	TArray<FIntPoint> targetIndices;
	targetIndices.Add(_index);
	pCombatSystem->CastSpell(m_SpellType, m_PlayerActions->GetSelectedTileIndex(), targetIndices);
}
