// Fill out your copyright notice in the Description page of Project Settings.


#include "AddTileAction.h"

#include "TacticalCombat/Core/PlayerActions.h"

void UAddTileAction::Execute(const FIntPoint& _index)
{
	AGrid* const pGrid = m_PlayerActions->GetGrid();
	if (pGrid == nullptr)
		return;
	
	if (pGrid->IsIndexValid(_index))
		return;
	
	ETileType tileType;
	FVector tileLocation =  pGrid->GetTileLocationFromGridIndex(_index.X, _index.Y);
	tileLocation = pGrid->TraceForGround(tileLocation, tileType);

	FTileData tileData;
	tileData.Index = _index;	
	tileData.Type = ETileType::Normal;
	tileData.Transform.SetLocation(tileLocation);
	tileData.Transform.SetRotation((pGrid->GetTileRotationFromGridIndex(_index.X, _index.Y).Quaternion()));
	tileData.Transform.SetScale3D(pGrid->GetTileScale());
	
	pGrid->AddGridTile(tileData);	
}
