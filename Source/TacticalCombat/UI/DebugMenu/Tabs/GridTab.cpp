// Fill out your copyright notice in the Description page of Project Settings.


#include "GridTab.h"

#include "Components/ComboBoxString.h"
#include "Kismet/GameplayStatics.h"
#include "TacticalCombat/Core/Grid/Grid.h"
#include "TacticalCombat/UI/DebugMenu/Elements/VectorWithNameSpinBox.h"
#include "TacticalCombat/UI/DebugMenu/Elements/Vector2DWithNameSpinBox.h"
#include "TacticalCombat/UI/DebugMenu/Elements/WithNameSpinBox.h"

void UGridTab::NativeConstruct()
{
	Super::NativeConstruct();

	m_GridInWorld = Cast<AGrid>(UGameplayStatics::GetActorOfClass(GetWorld(), AGrid::StaticClass()));
	_TrySetDefaultValues();
	_TrySpawnGrid();

	ComboBox_GridShape->OnSelectionChanged.AddDynamic(this, &UGridTab::_OnGridShapeSelectionChanged);
	SpinBox_Location->OnValueChanged.AddDynamic(this, &UGridTab::_OnLocationChanged);
	SpinBox_TileCount->OnValueChanged.AddDynamic(this, &UGridTab::_OnTileCountChanged);
	SpinBox_TileSize->OnValueChanged.AddDynamic(this, &UGridTab::_OnTileSizeChanged);
}

bool UGridTab::_TrySetDefaultValues()
{
	if (!m_GridInWorld.IsValid()) return false;

	const FVector& defaultLocation = m_GridInWorld->GetCenterLocation();
    const FVector2D& defaultTileCount = m_GridInWorld->GetTileCount();
    const FVector& defaultTileSize = m_GridInWorld->GetTileSize();
	EGridShape defaultGridShape = m_GridInWorld->GetGridShape();	
	
	ComboBox_GridShape->SetSelectedOption(StaticEnum<EGridShape>()->GetNameStringByValue((int64)defaultGridShape));
	SpinBox_Location->SetValue(defaultLocation);
	SpinBox_TileCount->SetValue(defaultTileCount);
	SpinBox_TileSize->SetValue(defaultTileSize);	
	return true;
}

bool UGridTab::_TrySpawnGrid()
{
	if (!m_GridInWorld.IsValid()) return false;
	const FVector& location = SpinBox_Location->GetValue();
	const FVector2D& tileCount = SpinBox_TileCount->GetValue();
	const FVector& tileSize = SpinBox_TileSize->GetValue();

	FName gridShapeName = FName(*ComboBox_GridShape->GetSelectedOption());
	EGridShape gridShape = static_cast<EGridShape>(StaticEnum<EGridShape>()->GetValueByName(gridShapeName));
	m_GridInWorld->SpawnGrid(location, tileSize, tileCount, gridShape);
	return true;
}

void UGridTab::_OnGridShapeSelectionChanged(FString _selectedItem, ESelectInfo::Type _selectionType)
{	
	if (!GetWorld()->GetTimerManager().IsTimerActive(m_hSpawnTimer))
	{
		GetWorld()->GetTimerManager().SetTimer(m_hSpawnTimer, this, &UGridTab::_ExecuteSpawnTimer, SpinBox_ReGenDelay->GetValue(), false);
	}
}

void UGridTab::_OnLocationChanged(const FVector& _value)
{
	if (!GetWorld()->GetTimerManager().IsTimerActive(m_hSpawnTimer))
	{
		GetWorld()->GetTimerManager().SetTimer(m_hSpawnTimer, this, &UGridTab::_ExecuteSpawnTimer, SpinBox_ReGenDelay->GetValue(), false);
	}
}

void UGridTab::_OnTileCountChanged(const FVector2D& _value)
{
	if (!GetWorld()->GetTimerManager().IsTimerActive(m_hSpawnTimer))
	{
		GetWorld()->GetTimerManager().SetTimer(m_hSpawnTimer, this, &UGridTab::_ExecuteSpawnTimer, SpinBox_ReGenDelay->GetValue(), false);
	}
}

void UGridTab::_OnTileSizeChanged(const FVector& _value)
{
	if (!GetWorld()->GetTimerManager().IsTimerActive(m_hSpawnTimer))
	{
		GetWorld()->GetTimerManager().SetTimer(m_hSpawnTimer, this, &UGridTab::_ExecuteSpawnTimer, SpinBox_ReGenDelay->GetValue(), false);
	}
}

void UGridTab::_ExecuteSpawnTimer()
{
	_TrySpawnGrid();
}
