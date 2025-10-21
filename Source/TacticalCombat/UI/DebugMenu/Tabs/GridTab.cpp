// Fill out your copyright notice in the Description page of Project Settings.


#include "GridTab.h"

#include "Components/CheckBox.h"
#include "Components/ComboBoxString.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "TacticalCombat/Core/Grid/Grid.h"
#include "TacticalCombat/Core/LevelScripts/EntryLevelScriptActor.h"
#include "TacticalCombat/Subsystems/LevelLoadingSubsystem.h"
#include "TacticalCombat/UI/DebugMenu/Elements/SpinBox/VectorWithNameSpinBox.h"
#include "TacticalCombat/UI/DebugMenu/Elements/SpinBox/Vector2DWithNameSpinBox.h"
#include "TacticalCombat/UI/DebugMenu/Elements/SpinBox/WithNameSpinBox.h"
#include "TacticalCombat/Misc/Enums.h"

const float DEBUG_DRAWING_DURATION = 0.1f;

void UGridTab::NativeConstruct()
{
	Super::NativeConstruct();
	
	m_GridInWorld = Cast<AGrid>(UGameplayStatics::GetActorOfClass(GetWorld(), AGrid::StaticClass()));
	_TrySetDefaultValues();

	GetWorld()->GetTimerManager().ClearTimer(m_hSpawnTimer);
	GetWorld()->GetTimerManager().ClearTimer(m_hDrawDebugTimer);
	
	GetWorld()->GetTimerManager().SetTimer(m_hDrawDebugTimer, this, &UGridTab::_DrawDebugLine, DEBUG_DRAWING_DURATION, true);
	
	ComboBox_Environment->OnSelectionChanged.AddDynamic(this, &UGridTab::_OnEnvironmentSelectionChanged);
	ComboBox_GridShape->OnSelectionChanged.AddDynamic(this, &UGridTab::_OnGridShapeSelectionChanged);	
	SpinBox_Location->OnValueChanged.AddDynamic(this, &UGridTab::_OnLocationChanged);
	SpinBox_TileCount->OnValueChanged.AddDynamic(this, &UGridTab::_OnTileCountChanged);
	SpinBox_TileSize->OnValueChanged.AddDynamic(this, &UGridTab::_OnTileSizeChanged);
	SpinBox_ZOffset->OnValueChanged.AddDynamic(this, &UGridTab::_OnZOffsetChanged);
	CheckBox_UseEnvironment->OnCheckStateChanged.AddDynamic(this, &UGridTab::_OnUseEnvironmentCheckStateChanged);
}

bool UGridTab::_TrySetDefaultValues()
{
	if (!m_GridInWorld.IsValid()) return false;

	AEntryLevelScriptActor* pEntryLevel = Cast<AEntryLevelScriptActor>(GetWorld()->GetLevelScriptActor());

	const FName& defaultLevelName = pEntryLevel->GetLevelName();
	const FVector& defaultLocation = m_GridInWorld->GetCenterLocation();
    const FVector2D& defaultTileCount = m_GridInWorld->GetTileCount();
    const FVector& defaultTileSize = m_GridInWorld->GetTileSize();
	float defaultZOffset = m_GridInWorld->GetZOffset();
	EGridShape defaultGridShape = m_GridInWorld->GetGridShape();	

	ComboBox_Environment->SetSelectedOption(defaultLevelName.ToString());
	ComboBox_GridShape->SetSelectedOption(StaticEnum<EGridShape>()->GetNameStringByValue((int64)defaultGridShape));
	SpinBox_Location->SetValue(defaultLocation);
	SpinBox_TileCount->SetValue(defaultTileCount);
	SpinBox_TileSize->SetValue(defaultTileSize);
	SpinBox_ZOffset->SetValue(defaultZOffset);
	CheckBox_UseEnvironment->SetIsChecked(true);
	return true;
}

bool UGridTab::_TrySpawnGrid()
{
	if (!m_GridInWorld.IsValid()) return false;
	const FVector& location = SpinBox_Location->GetValue();
	const FIntPoint& tileCount = FIntPoint(FMath::RoundToInt32(SpinBox_TileCount->GetValue().X), FMath::RoundToInt32(SpinBox_TileCount->GetValue().Y));
	const FVector& tileSize = SpinBox_TileSize->GetValue();

	FName gridShapeName = FName(*ComboBox_GridShape->GetSelectedOption());
	EGridShape gridShape = static_cast<EGridShape>(StaticEnum<EGridShape>()->GetValueByName(gridShapeName));
	m_GridInWorld->SpawnGrid(location, tileSize, tileCount, gridShape, CheckBox_UseEnvironment->IsChecked());
	return true;
}

void UGridTab::_OnEnvironmentSelectionChanged(FString _selectedItem, ESelectInfo::Type _selectionType)
{	
	ULevelLoadingSubsystem* pLevelLoadingSubsystem  =GetWorld()->GetGameInstance()->GetSubsystem<ULevelLoadingSubsystem>();
	pLevelLoadingSubsystem->LoadLevel(FName(*_selectedItem));
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

void UGridTab::_OnZOffsetChanged(float _value)
{
	if (!m_GridInWorld.IsValid())
		return;

	m_GridInWorld->SetZOffset(_value);
}

void UGridTab::_OnUseEnvironmentCheckStateChanged(bool _isChecked)
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

void UGridTab::_DrawDebugLine()
{
	if (!m_GridInWorld.IsValid())
		return;
	
	const FVector& gridCenterLocation = m_GridInWorld->GetCenterLocation();
	const FVector& gridBottomLeftLocation = m_GridInWorld->GetBottomLeftLocation();
	
	Text_GridCenter->SetText(gridCenterLocation.ToText());
	Text_GridBottomLeft->SetText(gridBottomLeftLocation.ToText());
	
	if (CheckBox_GridCenter->IsChecked())
	{
		const FVector& center = gridCenterLocation;
		constexpr float radius = 100.0f;
		constexpr int32 segments = 3;
		const FColor color = FColor::Orange;
		constexpr float thickness = 10.0f;
		DrawDebugSphere(GetWorld(), center, radius, segments, color, false, DEBUG_DRAWING_DURATION, 0, thickness);
	}

	if (CheckBox_GridBottomLeft->IsChecked())
	{
		const FVector& center = gridBottomLeftLocation;
		constexpr float radius = 100.0f;
		constexpr int32 segments = 3;
		const FColor color = FColor::Red;
		constexpr float thickness = 10.0f;
		DrawDebugSphere(GetWorld(), center, radius, segments, color, false, DEBUG_DRAWING_DURATION, 0, thickness);
	}

	if (CheckBox_GridBounds->IsChecked())
	{
		const FVector& center = gridCenterLocation;
		const FVector& extent = gridCenterLocation - gridBottomLeftLocation;
		const FColor color = FColor::Yellow;
		constexpr float thickness = 20.0f;
		DrawDebugBox(GetWorld(), center, extent, color, false, DEBUG_DRAWING_DURATION, 0, thickness);
	}

	const FVector cursorLocationOnGrid = m_GridInWorld->GetCursorLocationOnGrid(0);
	Text_MouseLocation->SetText(cursorLocationOnGrid.ToText());
	if (CheckBox_MouseLocation->IsChecked())
	{
		constexpr float radius = 15.0f;
		constexpr int32 segments = 5.0f;
		const FColor color = FColor::Yellow;
		constexpr float thickness = 5.0f;
		DrawDebugSphere(GetWorld(), cursorLocationOnGrid, radius, segments, color, false, DEBUG_DRAWING_DURATION, thickness);
	}

	const FIntPoint tileIndexUnderCursor = m_GridInWorld->GetTileIndexUnderCursor(0);
	Text_HoveredTile->SetText(FText::FromString(tileIndexUnderCursor.ToString()));
	if (CheckBox_HoveredTile->IsChecked())
	{
		const FTileData* const pFoundTileData = m_GridInWorld->GetGridTileMap().Find(tileIndexUnderCursor);
		if (pFoundTileData)
		{
			const FVector& center = pFoundTileData->Transform.GetLocation();;
			const FVector extent = FVector(35.0f, 35.0f, 5.0f);
			const FColor color = FColor::Orange;
			constexpr float thickness = 5.0f;
			DrawDebugBox(GetWorld(), center, extent, color, false, DEBUG_DRAWING_DURATION, 0, thickness);
		}
	}
}
