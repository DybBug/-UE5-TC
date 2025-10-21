// Fill out your copyright notice in the Description page of Project Settings.


#include "SetTileTypeAction.h"

#include "TacticalCombat/Core/PlayerActions.h"

void USetTileTypeAction::Execute(const FIntPoint& _index)
{
	AGrid* const pGrid = m_PlayerActions->GetGrid();
	if (!pGrid) return;
	if (!pGrid->IsIndexValid(_index)) return;

	const FTileData* pTileData = pGrid->GetGridTileMap().Find(_index);
	if (pTileData)
	{
		FTileData newTileData = *pTileData;
		newTileData.Type = m_TileType;
		pGrid->AddGridTile(newTileData);
	}
}
