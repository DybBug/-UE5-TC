// Fill out your copyright notice in the Description page of Project Settings.


#include "RemoveTileAction.h"

#include "TacticalCombat/Core/PlayerActions.h"
#include "TacticalCombat/Core/Grid/Grid.h"

void URemoveTileAction::Execute(const FIntPoint& _index)
{
	if (!m_PlayerActions.IsValid())
		return;

	AGrid* const pGrid = m_PlayerActions->GetGrid();
	if (pGrid == nullptr)
		return;

	if (!pGrid->IsIndexValid(_index))
		return;

	pGrid->RemoveGridTile(_index);
}
