// Fill out your copyright notice in the Description page of Project Settings.


#include "PathFindingTab.h"

#include "Components/CheckBox.h"
#include "TacticalCombat/Core/Grid/Other/DebugTextAndColorOnTiles.h"
#include "TacticalCombat/Misc/Enums.h"

void UPathFindingTab::NativeConstruct()
{
	Super::NativeConstruct();

	if (!m_DebugTextOnTiles)
	{
		m_DebugTextOnTiles = NewObject<UDebugTextAndColorOnTiles>(this);
		m_DebugTextOnTiles->Initialize();
	}

	CheckBox_IsShowIndicesOnTiles->SetIsChecked(m_DebugTextOnTiles->HasTileDebugFlag(ETileDebugFlags::TileIndices));
	CheckBox_IsShowIndicesOnTiles->OnCheckStateChanged.AddDynamic(this, &UPathFindingTab::OnIsShowIndicesOnTilesCheckStateChanged);

	CheckBox_IsShowTileType->SetIsChecked(m_DebugTextOnTiles->HasTileDebugFlag(ETileDebugFlags::TileType));
	CheckBox_IsShowTileType->OnCheckStateChanged.AddDynamic(this, &UPathFindingTab::OnIsShowTileTypeCheckStateChanged);

	CheckBox_IsShowCostToEnterTile->SetIsChecked(m_DebugTextOnTiles->HasTileDebugFlag(ETileDebugFlags::CostToEnterTile));
	CheckBox_IsShowCostToEnterTile->OnCheckStateChanged.AddDynamic(this, &UPathFindingTab::OnIsShowCostToEnterTileCheckStateChanged);

	CheckBox_IsShowMinCostToTarget->SetIsChecked(m_DebugTextOnTiles->HasTileDebugFlag(ETileDebugFlags::MinCostToTarget));
	CheckBox_IsShowMinCostToTarget->OnCheckStateChanged.AddDynamic(this, &UPathFindingTab::OnIsShowMinCostToTargetCheckStateChanged);

	CheckBox_IsShowCostFromStart->SetIsChecked(m_DebugTextOnTiles->HasTileDebugFlag(ETileDebugFlags::CostFromStart));
	CheckBox_IsShowCostFromStart->OnCheckStateChanged.AddDynamic(this, &UPathFindingTab::OnIsShowCostFromStartCheckStateChanged);

	CheckBox_IsShowSortOrder->SetIsChecked(m_DebugTextOnTiles->HasTileDebugFlag(ETileDebugFlags::SortOrder));
	CheckBox_IsShowSortOrder->OnCheckStateChanged.AddDynamic(this, &UPathFindingTab::OnIsShowSortOrderCheckStateChanged);

	CheckBox_IsShowDiscoveredTiles->SetIsChecked(m_DebugTextOnTiles->HasTileDebugFlag(ETileDebugFlags::DiscoveredTiles));
	CheckBox_IsShowDiscoveredTiles->OnCheckStateChanged.AddDynamic(this, &UPathFindingTab::OnIsShowDiscoveredTilesCheckStateChanged);

	CheckBox_IsShowAnalyzedTiles->SetIsChecked(m_DebugTextOnTiles->HasTileDebugFlag(ETileDebugFlags::AnalysedTiles));
	CheckBox_IsShowAnalyzedTiles->OnCheckStateChanged.AddDynamic(this, &UPathFindingTab::OnIsShowAnalyzedTilesCheckStateChanged);
}

void UPathFindingTab::OnIsShowIndicesOnTilesCheckStateChanged(bool _bIsChecked)
{
	m_DebugTextOnTiles->SetTileDebugFlag(ETileDebugFlags::TileIndices, _bIsChecked);
}

void UPathFindingTab::OnIsShowTileTypeCheckStateChanged(bool _bIsChecked)
{
	m_DebugTextOnTiles->SetTileDebugFlag(ETileDebugFlags::TileType, _bIsChecked);
}

void UPathFindingTab::OnIsShowCostToEnterTileCheckStateChanged(bool _bIsChecked)
{
	m_DebugTextOnTiles->SetTileDebugFlag(ETileDebugFlags::CostToEnterTile, _bIsChecked);	
}

void UPathFindingTab::OnIsShowMinCostToTargetCheckStateChanged(bool _bIsChecked)
{
	m_DebugTextOnTiles->SetTileDebugFlag(ETileDebugFlags::MinCostToTarget, _bIsChecked);
}

void UPathFindingTab::OnIsShowCostFromStartCheckStateChanged(bool _bIsChecked)
{
	m_DebugTextOnTiles->SetTileDebugFlag(ETileDebugFlags::CostFromStart, _bIsChecked);
}

void UPathFindingTab::OnIsShowSortOrderCheckStateChanged(bool _bIsChecked)
{
	m_DebugTextOnTiles->SetTileDebugFlag(ETileDebugFlags::SortOrder, _bIsChecked);
}

void UPathFindingTab::OnIsShowDiscoveredTilesCheckStateChanged(bool _bIsChecked)
{	
	m_DebugTextOnTiles->SetShowTileStates(_bIsChecked, CheckBox_IsShowAnalyzedTiles->IsChecked());
}

void UPathFindingTab::OnIsShowAnalyzedTilesCheckStateChanged(bool _bIsChecked)
{
	m_DebugTextOnTiles->SetShowTileStates(CheckBox_IsShowDiscoveredTiles->IsChecked(), _bIsChecked);
}
