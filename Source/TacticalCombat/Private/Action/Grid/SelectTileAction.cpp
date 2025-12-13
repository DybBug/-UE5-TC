// Fill out your copyright notice in the Description page of Project Settings.


#include "Action/Grid/SelectTileAction.h"
#include "Core/PlayerActions.h"
#include "Shared/SharedEnums.h"

void USelectTileAction::Execute(const FIntPoint& _index)
{
	if (!m_PlayerActions.IsValid())
		return;
	
	AGrid* const pGrid =  m_PlayerActions->GetGrid();

	const FIntPoint& selectedTileIndex = m_PlayerActions->GetSelectedTileIndex();
	if (selectedTileIndex != _index)
	{
		pGrid->RemoveStateFromTile(selectedTileIndex, ETileStateFlags::Selected);
		
		m_PlayerActions->SetSelectedTileIndex(_index);
		pGrid->AddStateToTile(_index, ETileStateFlags::Selected);
	}
	else
	{
		pGrid->RemoveStateFromTile(selectedTileIndex, ETileStateFlags::Selected);
		m_PlayerActions->SetSelectedTileIndex(FIntPoint(-999, -999));
	}
}
