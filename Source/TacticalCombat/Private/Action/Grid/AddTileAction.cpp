// Fill out your copyright notice in the Description page of Project Settings.


#include "Action/Grid/AddTileAction.h"
#include "Core/PlayerActions.h"
#include "Grid/Grid.h"

void UAddTileAction::Execute(const FIntPoint& _index)
{
	if (!m_PlayerActions.IsValid())
		return;
	
	AGrid* const pGrid = m_PlayerActions->GetGrid();	
	
	ETileType tileType;
	FVector tileLocation =  pGrid->GetTileLocationFromGridIndex(_index.X, _index.Y);
	tileLocation = pGrid->TraceForGround(tileLocation, tileType);

	FTileData tileData;
	tileData.Index = _index;	
	tileData.Type = ETileType::Normal;
	tileData.Transform.SetLocation(tileLocation);
	tileData.Transform.SetRotation((pGrid->GetTileRotationFromGridIndex(_index.X, _index.Y).Quaternion()));
	tileData.Transform.SetScale3D(pGrid->GetTileScale());
	
	pGrid->AddGridTileWithNotify(tileData);	
}
