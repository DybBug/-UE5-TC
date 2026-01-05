// Fill out your copyright notice in the Description page of Project Settings.


#include "Action/Grid/RemoveTileAction.h"

#include "Core/PlayerActions.h"
#include "Grid/Grid.h"

void URemoveTileAction::Execute(const FIntPoint& _index)
{
	if (!m_PlayerActions.IsValid())
		return;

	AGrid* const pGrid = m_PlayerActions->GetGrid();
	if (pGrid == nullptr)
		return;

	if (!pGrid->IsIndexValid(_index))
		return;

	pGrid->RemoveGridTileWithNotify(_index);
}
