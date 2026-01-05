// Fill out your copyright notice in the Description page of Project Settings.


#include "Action/Grid/SelectTileAndUnitAction.h"
#include "Core/PlayerActions.h"
#include "Shared/SharedEnums.h"
#include "Grid/Grid.h"

void USelectTileAndUnitAction::Execute(const FIntPoint& _index)
{
	if (!m_PlayerActions.IsValid())
		return;
	
	m_PlayerActions->SelectTileAndUnit(_index);
}
