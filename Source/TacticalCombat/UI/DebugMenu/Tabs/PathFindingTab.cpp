// Fill out your copyright notice in the Description page of Project Settings.


#include "PathFindingTab.h"

#include "Components/CheckBox.h"
#include "TacticalCombat/Core/Grid/Other/DebugTextOnTiles.h"
#include "TacticalCombat/Misc/Enums.h"

void UPathFindingTab::NativeConstruct()
{
	Super::NativeConstruct();

	if (!m_DebugTextOnTiles)
	{
		m_DebugTextOnTiles = NewObject<UDebugTextOnTiles>(this);
		m_DebugTextOnTiles->Initialize();
	}

	CheckBox_IsShowIndicesOnTiles->SetIsChecked(m_DebugTextOnTiles->HasTileDebugFlag(ETileDebugFlags::TileIndices));
	CheckBox_IsShowIndicesOnTiles->OnCheckStateChanged.AddDynamic(this, &UPathFindingTab::OnIsShowIndicesOnTilesCheckStateChanged);

	CheckBox_IsShowCostToEnterTile->SetIsChecked(m_DebugTextOnTiles->HasTileDebugFlag(ETileDebugFlags::CostToEnterTile));
	CheckBox_IsShowCostToEnterTile->OnCheckStateChanged.AddDynamic(this, &UPathFindingTab::OnIsShowCostToEnterTileCheckStateChanged);

	CheckBox_IsShowMinCostToTarget->SetIsChecked(m_DebugTextOnTiles->HasTileDebugFlag(ETileDebugFlags::MinCostToTarget));
	CheckBox_IsShowMinCostToTarget->OnCheckStateChanged.AddDynamic(this, &UPathFindingTab::OnIsShowMinCostToTargetCheckStateChanged);

	CheckBox_IsShowCostFromStart->SetIsChecked(m_DebugTextOnTiles->HasTileDebugFlag(ETileDebugFlags::CostFromStart));
	CheckBox_IsShowCostFromStart->OnCheckStateChanged.AddDynamic(this, &UPathFindingTab::OnIsShowCostFromStartCheckStateChanged);

	CheckBox_IsShowSortOrder->SetIsChecked(m_DebugTextOnTiles->HasTileDebugFlag(ETileDebugFlags::SortOrder));
	CheckBox_IsShowSortOrder->OnCheckStateChanged.AddDynamic(this, &UPathFindingTab::OnIsShowSortOrderCheckStateChanged);
}

void UPathFindingTab::OnIsShowIndicesOnTilesCheckStateChanged(bool _bIsChecked)
{
	if (_bIsChecked)
	{
		m_DebugTextOnTiles->AddTileDebugFlag(ETileDebugFlags::TileIndices);	
	}
	else
	{
		m_DebugTextOnTiles->RemoveTileDebugFlag(ETileDebugFlags::TileIndices);	
	}
}

void UPathFindingTab::OnIsShowCostToEnterTileCheckStateChanged(bool _bIsChecked)
{
	if (_bIsChecked)
	{
		m_DebugTextOnTiles->AddTileDebugFlag(ETileDebugFlags::CostToEnterTile);	
	}
	else
	{
		m_DebugTextOnTiles->RemoveTileDebugFlag(ETileDebugFlags::CostToEnterTile);	
	}	
}

void UPathFindingTab::OnIsShowMinCostToTargetCheckStateChanged(bool _bIsChecked)
{
	if (_bIsChecked)
	{
		m_DebugTextOnTiles->AddTileDebugFlag(ETileDebugFlags::MinCostToTarget);	
	}
	else
	{
		m_DebugTextOnTiles->RemoveTileDebugFlag(ETileDebugFlags::MinCostToTarget);	
	}
}

void UPathFindingTab::OnIsShowCostFromStartCheckStateChanged(bool _bIsChecked)
{
	if (_bIsChecked)
	{
		m_DebugTextOnTiles->AddTileDebugFlag(ETileDebugFlags::CostFromStart);	
	}
	else
	{
		m_DebugTextOnTiles->RemoveTileDebugFlag(ETileDebugFlags::CostFromStart);	
	}
}

void UPathFindingTab::OnIsShowSortOrderCheckStateChanged(bool _bIsChecked)
{
	if (_bIsChecked)
	{
		m_DebugTextOnTiles->AddTileDebugFlag(ETileDebugFlags::SortOrder);	
	}
	else
	{
		m_DebugTextOnTiles->RemoveTileDebugFlag(ETileDebugFlags::SortOrder);	
	}
}
