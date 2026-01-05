// Fill out your copyright notice in the Description page of Project Settings.


#include "Action/Combat/AddRemoveUnitAction.h"

#include "Combat/CombatSystem.h"
#include "Core/PlayerActions.h"
#include "Grid/Grid.h"
#include "Unit/Unit.h"

void UAddRemoveUnitAction::Execute(const FIntPoint& _index)
{
	if (m_PlayerActions == nullptr)
		return;
	
	if (m_bIsAddingUnit)
	{
		if (m_PlayerActions->GetGrid()->IsIndexValid((_index)))
		{
			const TMap<FIntPoint, FTileData>& girdTileMap =  m_PlayerActions->GetGrid()->GetGridTileMap();
			if (const FTileData* pTile = girdTileMap.Find(_index))
			{
				if (m_PlayerActions->GetGrid()->IsWalkableTile(_index) && pTile->UnitOnTile.IsValid())
				{
					m_PlayerActions->GetCombatSystem()->RemoveUnitFromCombat(pTile->UnitOnTile.Get(), true);
				}
			}	

			AUnit* pSpawnedUnit = AUnit::Spawn(GetWorld(), m_UnitType, m_PlayerActions->GetGrid());
			m_PlayerActions->GetCombatSystem()->AddUnitInCombat(pSpawnedUnit, _index);
		}
	}
	else
	{
		const TMap<FIntPoint, FTileData>& girdTileMap =  m_PlayerActions->GetGrid()->GetGridTileMap();
		if (const FTileData* pTile = girdTileMap.Find(_index))
		{
			if (pTile->UnitOnTile.IsValid())
			{
				m_PlayerActions->GetCombatSystem()->RemoveUnitFromCombat(pTile->UnitOnTile.Get(), true);
			}
		}		
	}
}
