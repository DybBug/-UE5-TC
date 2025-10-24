// Fill out your copyright notice in the Description page of Project Settings.


#include "PathFindingTab.h"

#include "Components/CheckBox.h"
#include "TacticalCombat/Core/Grid/Other/DebugTextOnTiles.h"

void UPathFindingTab::NativeConstruct()
{
	Super::NativeConstruct();

	if (!m_DebugTextOnTiles)
	{
		m_DebugTextOnTiles = NewObject<UDebugTextOnTiles>(this);
		m_DebugTextOnTiles->Initialize();
	}

	CheckBox_IsShowIndicesOnTiles->SetIsChecked(m_DebugTextOnTiles->IsShowTileIndices());
	CheckBox_IsShowIndicesOnTiles->OnCheckStateChanged.AddDynamic(this, &UPathFindingTab::OnCheckStateChanged);
}

void UPathFindingTab::OnCheckStateChanged(bool _bIsChecked)
{
	m_DebugTextOnTiles->SetShowTileIndices(_bIsChecked);	
}
