// Fill out your copyright notice in the Description page of Project Settings.


#include "ModifyTileHeightAction.h"

#include "TacticalCombat/Core/PlayerActions.h"

void UModifyTileHeightAction::Execute(const FIntPoint& _index)
{
	AGrid* const pGrid = m_PlayerActions->GetGrid();
	if (!pGrid) return;
	if (!pGrid->IsIndexValid(_index)) return;

	const FTileData* pTileData = pGrid->GetGridTileMap().Find(_index);
	if (pTileData)
	{
		FVector tileLocationOffset = FVector::ZeroVector;
		tileLocationOffset.Z = pGrid->GetTileSize().Z * (m_bIsIncrease ? +1.0f : -1.0f);
		
		FTileData newTileData;
		newTileData.Index = pTileData->Index;
		newTileData.Type = pTileData->Type;
		newTileData.Transform.SetLocation(pTileData->Transform.GetLocation() + tileLocationOffset);
		newTileData.Transform.SetRotation(pTileData->Transform.GetRotation());
		newTileData.Transform.SetScale3D(pTileData->Transform.GetScale3D());
		newTileData.StateMask =pTileData->StateMask;

		pGrid->AddGridTile(newTileData);
	}
	
}
