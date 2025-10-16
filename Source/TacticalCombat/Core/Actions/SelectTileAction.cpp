// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectTileAction.h"

#include "TacticalCombat/Core/PlayerActions.h"

void USelectTileAction::BeginDestroy()
{
	Super::BeginDestroy();
	Execute(FIntPoint(-999, -999));
}

void USelectTileAction::Execute(const FIntPoint& _index)
{
	AGrid* const pGrid =  m_PlayerActions->GetGrid();

	const FIntPoint& selectedTileIndex = m_PlayerActions->GetSelectedTileIndex();
	if (selectedTileIndex != _index)
	{
		pGrid->RemoveStateToTile(selectedTileIndex, ETileStateFlags::Selected);
		
		m_PlayerActions->SetSelectedTileIndex(_index);
		pGrid->AddStateToTile(_index, ETileStateFlags::Selected);
	}
	else
	{
		pGrid->RemoveStateToTile(selectedTileIndex, ETileStateFlags::Selected);
		m_PlayerActions->SetSelectedTileIndex(FIntPoint(-999, -999));
	}
}
