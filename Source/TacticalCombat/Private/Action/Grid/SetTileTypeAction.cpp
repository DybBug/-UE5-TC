// Fill out your copyright notice in the Description page of Project Settings.


#include "Action/Grid/SetTileTypeAction.h"

#include "Core/PlayerActions.h"
#include "Grid/Grid.h"

void USetTileTypeAction::Execute(const FIntPoint& _index)
{
	if (m_PlayerActions == nullptr)
		return;
	
	AGrid* const pGrid = m_PlayerActions->GetGrid();
	if (!pGrid) return;
	if (!pGrid->IsIndexValid(_index)) return;

	const FTileData* pTileData = pGrid->GetGridTileMap().Find(_index);
	if (pTileData)
	{
		FTileData newTileData = *pTileData;
		newTileData.Type = m_TileType;
		pGrid->AddGridTileWithNotify(newTileData);
	}
}
