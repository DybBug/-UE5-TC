// Fill out your copyright notice in the Description page of Project Settings.


#include "Action/Grid/ShowSpellRangePatternAction.h"

#include "Combat/CombatSystem.h"
#include "Core/PlayerActions.h"
#include "Grid/Grid.h"

void UShowSpellRangePatternAction::Execute(const FIntPoint& _index)
{
	Super::Execute(_index);
	ShowSpellRangePattern();
}

void UShowSpellRangePatternAction::ShowSpellRangePattern()
{
	if (m_PlayerActions == nullptr)
		return;

	AGrid* const pGrid = m_PlayerActions->GetGrid();
	pGrid->ClearStateFromTiles(ETileStateFlags::SpellRange);

	const FIntPoint& selectedTileIndex = m_PlayerActions->GetSelectedTileIndex();
	if (pGrid->IsIndexValid(selectedTileIndex))
	{
		const TArray<FIntPoint>& rangeIndices = m_PlayerActions->GetCombatSystem()->GetSpellRangeIndices(selectedTileIndex, m_Cast);
		for (const FIntPoint& tileIndex : rangeIndices)
		{
			pGrid->AddStateToTileWithNotify(tileIndex, ETileStateFlags::SpellRange);
		}
	}
}
