// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid.h"

#include "GridModifier.h"
#include "GridPathfinding.h"
#include "GridVisual.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TacticalCombat/Libraries/GridLibrary.h"
#include "TacticalCombat/Libraries/UtilityLibrary.h"
#include "TacticalCombat/Misc/Defines.h"
#include "TacticalCombat/Misc/Enums.h"

bool AGrid::IsWalkableTile(ETileType _type)
{
	switch (_type)
	{
		case ETileType::None:
		case ETileType::Obstacle:
		{
			return false;
		}
		case ETileType::Normal:
		case ETileType::DoubleCost:
		case ETileType::TripleCost:
		case ETileType::FlyingUnitsOnly:
		{
			return true;
		}
		default:
		{			
			checkf(false, TEXT("Invalid ETileType : %s"), *StaticEnum<ETileType>()->GetNameStringByValue((uint8)_type));
			return false;
		}
	}
}

// Sets default values
AGrid::AGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	m_SceneComponent = CreateDefaultSubobject<USceneComponent>("Scene Component");
	RootComponent = m_SceneComponent;

	m_ChildActorGridVisual = CreateDefaultSubobject<UChildActorComponent>("Grid Visual ChildActor Component");
	m_ChildActorGridVisual->SetChildActorClass(AGridVisual::StaticClass());
	m_ChildActorGridVisual->SetupAttachment(m_SceneComponent);
	
	m_ChildActorGridPathFinding = CreateDefaultSubobject<UChildActorComponent>("Grid Pathfinding ChildActor Component");
    m_ChildActorGridPathFinding->SetChildActorClass(AGridPathfinding::StaticClass());
    m_ChildActorGridPathFinding->SetupAttachment(m_SceneComponent);
}

void AGrid::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
#if WITH_EDITOR
	// 에디터 전용 미리보기
	m_GridVisual = Cast<AGridVisual>(m_ChildActorGridVisual->GetChildActor());
	if (m_GridVisual == nullptr)
		return;
	
	m_GridPathfinding = Cast<AGridPathfinding>(m_ChildActorGridPathFinding->GetChildActor());
	if (m_GridPathfinding == nullptr)
		return;
	
	m_GridPathfinding->SetGrid(this);
	
	SpawnGrid(GetActorLocation(), m_TileSize, m_TileCount, m_Shape, true);
#endif
}

// Called when the game starts or when spawned
void AGrid::BeginPlay()
{
	Super::BeginPlay();
	
	m_GridVisual = Cast<AGridVisual>(m_ChildActorGridVisual->GetChildActor());
	m_GridPathfinding = Cast<AGridPathfinding>(m_ChildActorGridPathFinding->GetChildActor());
	
	m_GridPathfinding->SetGrid(this);
	
	SpawnGrid(GetActorLocation(), m_TileSize, m_TileCount, m_Shape, true);
}

void AGrid::SpawnGrid(const FVector& _centerLocation, const FVector& _tileSize, const FIntPoint& _tileCount, EGridShape _shape, bool _bIsUseEnvironment)
{	
	m_CenterLocation = _centerLocation;
	m_TileSize = _tileSize;
	m_Shape = _shape;
	m_TileCount = _tileCount;
	
	DestroyGrid();

	m_GridVisual->InitializeGridVisual(this);
	
	_CalculateCenterAndBottomLeft(m_CenterLocation, m_GridBottomLeftCornerLocation);

	// Make Tiles
	int32 rowCount = m_TileCount.X;
	for (int32 row = 0; row < rowCount; ++row)
	{
		int32 startCol = (m_Shape != EGridShape::Hexagon) ? 0 : (UUtilityLibrary::IsIntEven(row) ? 0 : 1);
		int32 colCount = (m_Shape != EGridShape::Hexagon) ? m_TileCount.Y : m_TileCount.Y * 2;
		int32 colStep = (m_Shape != EGridShape::Hexagon) ? 1 : 2;
		for (int32 col = startCol; col < colCount; col += colStep)
		{			
			FTransform tileTransform;
			tileTransform.SetLocation(GetTileLocationFromGridIndex(row, col));
			tileTransform.SetRotation(GetTileRotationFromGridIndex(row, col).Quaternion());
			tileTransform.SetScale3D(GetTileScale());

			ETileType tracedTileType = ETileType::Normal;
			if (_bIsUseEnvironment)
			{					
				FVector hitLocation = TraceForGround(tileTransform.GetLocation(), tracedTileType);
				tileTransform.SetLocation(hitLocation);
			}
			
			FTileData newTileData = FTileData{FIntPoint(row, col), tracedTileType, tileTransform};
			AddGridTile(newTileData);		
		}
	}
	
}

void AGrid::DestroyGrid()
{
	m_GridTileMap.Empty();
	if (m_GridVisual)
	{
		m_GridVisual->DestroyGridVisual();
		if (OnGridDestroyed.IsBound())
		{
			OnGridDestroyed.Broadcast();
		}
	}
}

FGridShapeData AGrid::GetGridShapeData() const
{
	return UGridLibrary::GetGridShape(m_Shape);
}

FVector AGrid::GetCursorLocationOnGrid(int _playerIndex)
{
	APlayerController* pPlayerController = UGameplayStatics::GetPlayerController(GetWorld(), _playerIndex);
	if (pPlayerController)
	{
		FHitResult hitResult;
		bool isSucceeded = pPlayerController->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(GTC_Grid), true, hitResult);
		if (isSucceeded)
		{
			return hitResult.ImpactPoint;
		}

		isSucceeded = pPlayerController->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(GTC_GroundAndGridModifier), true, hitResult);
		if (isSucceeded)
		{
			return hitResult.ImpactPoint;
		}

		FVector worldLocation, worldDirection;	
		pPlayerController->DeprojectMousePositionToWorld(worldLocation, worldDirection);

		float t;
		FVector intersection;
		const FVector lineStart = worldLocation;
		const FVector lineEnd = worldLocation + ( worldDirection * 999999.0f);
		const FPlane plane = UKismetMathLibrary::MakePlaneFromPointAndNormal(m_CenterLocation, FVector::UpVector);

		isSucceeded = UKismetMathLibrary::LinePlaneIntersection(lineStart, lineEnd, plane, t, intersection);
		if (isSucceeded)
		{
			return intersection;
		}
		return FVector(-999.0f);
	}

	return FVector(-999.0f);
}

FIntPoint AGrid::GetTileIndexFromWorldLocation(const FVector& _location)
{
	const FVector locationOnGrid = _location - m_GridBottomLeftCornerLocation;
	switch (m_Shape)
	{
		case EGridShape::None:
		{
			return FIntPoint(-999);
		}
		case EGridShape::Square:
		{
			const FVector snappedLocationOnGird = UUtilityLibrary::SnapVectorToVector(locationOnGrid, m_TileSize);
			const FVector index = snappedLocationOnGird / m_TileSize;
			return FIntPoint(index.X, index.Y);

		}
		case EGridShape::Hexagon:
		{
			const FVector snappedLocationOnGrid =  UUtilityLibrary::SnapVectorToVector(locationOnGrid * FVector(1.0f, 2.0f, 1.0f), m_TileSize * FVector(0.75f, 0.25f, 1.0f));
			const FVector tempIndex = snappedLocationOnGrid / (m_TileSize * FVector(0.75f, 1.0f, 1.0f));
			if (UUtilityLibrary::IsFloatEven(tempIndex.X))
			{
				const int32 row = FMath::TruncToInt32(tempIndex.X);
				const int32 col = FMath::TruncToInt32( FMath::RoundToInt(tempIndex.Y * 0.5f) * 2.0f);
				return FIntPoint(row, col);
			}
			else
			{
				const int32 row = FMath::TruncToInt32(tempIndex.X);
				const int32 col = FMath::TruncToInt32( FMath::FloorToInt(tempIndex.Y * 0.5f) * 2.0f + 1.0f);
				return FIntPoint(row, col);
			}
		}
		case EGridShape::Triangle:
		{
			const FVector snappedLocationOnGird = UUtilityLibrary::SnapVectorToVector(locationOnGrid, m_TileSize / FVector(1.0f, 2.0f, 1.0f));
			const FVector index = (snappedLocationOnGird / m_TileSize) * FVector(1.0f, 2.0f, 1.0f);
			return FIntPoint(index.X, index.Y);
		}
		default:
		{
			return FIntPoint(-999);
		}
	}
}

FIntPoint AGrid::GetTileIndexUnderCursor(int _playerIndex)
{
	const FVector location =  GetCursorLocationOnGrid(_playerIndex);
	return GetTileIndexFromWorldLocation(location);
}

void AGrid::AddGridTile(const FTileData& _tileData)
{
	m_GridTileMap.Add(_tileData.Index, _tileData);
	m_GridVisual->UpdateTileVisual(_tileData);
	if (OnTileDataUpdated.IsBound())
	{
		OnTileDataUpdated.Broadcast(_tileData.Index);
	}
}

void AGrid::RemoveGridTile(const FIntPoint& _tileIndex)
{
	bool isRemove = m_GridTileMap.Remove(_tileIndex) > 0;
	if (isRemove)
	{
		FTileData tileData;
		tileData.Index = _tileIndex;
		tileData.Type = ETileType::None;
		m_GridVisual->UpdateTileVisual(tileData);

		if (OnTileDataUpdated.IsBound())
		{
			OnTileDataUpdated.Broadcast(_tileIndex);
		}
	}	
}

void AGrid::AddStateToTile(const FIntPoint& _tileIndex, const ETileStateFlags _stateFlag)
{
	FTileData* const pTileData =  m_GridTileMap.Find(_tileIndex);
	if (pTileData)
	{
		pTileData->StateMask |= static_cast<int32>(_stateFlag);

		TArray<FIntPoint> allTilesWithStateFlag  = GetAllTilesWithStateFlag(_stateFlag);
		allTilesWithStateFlag.Add(_tileIndex);
		m_TileIndicesByStateFlag.Add(_stateFlag, allTilesWithStateFlag);		
		
		m_GridVisual->UpdateTileVisual(*pTileData);

		if (OnTileDataUpdated.IsBound())
		{
			OnTileDataUpdated.Broadcast(_tileIndex);
		}
	}
}

void AGrid::RemoveStateFromTile(const FIntPoint& _tileIndex, const ETileStateFlags _stateFlag)
{
	FTileData* const pTileData =  m_GridTileMap.Find(_tileIndex);
	if (pTileData)
	{
		pTileData->StateMask &= ~static_cast<int32>(_stateFlag);

		TArray<FIntPoint> allTilesWithStateFlag  = GetAllTilesWithStateFlag(_stateFlag);
		allTilesWithStateFlag.Remove(_tileIndex);
		m_TileIndicesByStateFlag.Add(_stateFlag, allTilesWithStateFlag);	
		
		m_GridVisual->UpdateTileVisual(*pTileData);

		if (OnTileDataUpdated.IsBound())
		{
			OnTileDataUpdated.Broadcast(_tileIndex);
		}
	}
}

bool AGrid::IsIndexValid(const FIntPoint& _tileIndex)
{
	return m_GridTileMap.Contains(_tileIndex);
}

FVector AGrid::TraceForGround(const FVector& _location, ETileType& _hitTileType)
{
	TArray<FHitResult> hitResults;
	const FVector& startLocation = _location + FVector(0.0f, 0.0f, 1000.0f);
	const FVector& endLocation = _location + FVector(0.0f, 0.0f, -1000.0f);
	const float radius = m_TileSize.X / (m_Shape == EGridShape::Triangle ? 5.0f : 3.0f);
	ETraceTypeQuery traceChannel = UEngineTypes::ConvertToTraceType(GTC_GroundAndGridModifier);
	TArray<AActor*> ignoreActors = TArray<AActor*>();
	EDrawDebugTrace::Type drawDebugType = EDrawDebugTrace::None;
	
	UKismetSystemLibrary::SphereTraceMulti(GetWorld(), startLocation, endLocation, radius, traceChannel, false, ignoreActors, drawDebugType, hitResults, true);
	if (hitResults.Num() <= 0)
	{
		_hitTileType = ETileType::None;
		return FVector::ZeroVector;
	}

	_hitTileType = ETileType::Normal;
	FVector resultLocation = _location;
	for (const FHitResult hitResult : hitResults)
	{			
		if (AGridModifier* pGridModifier = Cast<AGridModifier>(hitResult.GetActor()))
		{
			_hitTileType = pGridModifier->GetType();
			if (!IsWalkableTile(_hitTileType) || pGridModifier->GetIsUsingTileHeight())
			{
				resultLocation.Z = UKismetMathLibrary::GridSnap_Float(hitResult.ImpactPoint.Z, m_TileSize.Z);
				break;
			}			
		}
		resultLocation.Z = UKismetMathLibrary::GridSnap_Float(hitResult.ImpactPoint.Z, m_TileSize.Z);
	}			
	return resultLocation;	
}

FVector AGrid::GetTileLocationFromGridIndex(int _row, int _col)
{
	FVector sizeOffset = FVector::ZeroVector;
	switch (m_Shape)
	{
		case EGridShape::Square:
		{
			sizeOffset = FVector(1.0f, 1.0f, 0.0f);
			break;
		}
		case EGridShape::Hexagon:
		{
			sizeOffset = FVector(0.75f, 0.5f, 0.0f);
			break;
		}
		case EGridShape::Triangle:
		{
			sizeOffset = FVector(1.0f, 0.5f, 0.0f);
			break;
		}
		case EGridShape::None:
			default:
		{
			sizeOffset = FVector::ZeroVector;
			break;
		}
	}
	
	FVector adjustedTileSize =  m_TileSize * sizeOffset;
	return m_GridBottomLeftCornerLocation + FVector(adjustedTileSize.X * _row, adjustedTileSize.Y * _col, adjustedTileSize.Z);
}

FRotator AGrid::GetTileRotationFromGridIndex(int _row, int _col)
{
	// 오직 삼각형만 회전
	if (m_Shape != EGridShape::Triangle)
		return FRotator::ZeroRotator;

	if (UUtilityLibrary::IsIntEven (_col) && !UUtilityLibrary::IsIntEven (_row))
		return FRotator(0.0f, 180.0f, 0.0f);

	if (!UUtilityLibrary::IsIntEven (_col) && UUtilityLibrary::IsIntEven (_row))
		return FRotator(0.0f, 180.0f, 0.0f);

	return FRotator::ZeroRotator;
}

FVector AGrid::GetTileScale()
{
	return m_TileSize / GetGridShapeData().MeshSize;
}

TArray<FIntPoint> AGrid::GetAllTilesWithStateFlag(const ETileStateFlags _stateFlag)
{
	TArray<FIntPoint>* pIndices = m_TileIndicesByStateFlag.Find(_stateFlag);
	if (pIndices == nullptr)
		return TArray<FIntPoint>();
	
	return *pIndices;
}

void AGrid::ClearStateFromTiles(const ETileStateFlags _stateFlag)
{
	TArray<FIntPoint> indices = GetAllTilesWithStateFlag(_stateFlag);
	for (const FIntPoint& index : indices)
	{
		RemoveStateFromTile(index, _stateFlag);
	}
}

bool AGrid::IsWalkableTile(const FIntPoint& _index)
{
	const FTileData* pTile = m_GridTileMap.Find(_index);
	if (!pTile) return false;

	return IsWalkableTile(pTile->Type);
}

void AGrid::_CalculateCenterAndBottomLeft(FVector& _center, FVector& _bottomLeft)
{
	switch (m_Shape)
	{
		case EGridShape::None:
		{
			break;
		}
		case EGridShape::Square:
		{
			_center =  UUtilityLibrary::SnapVectorToVector(m_CenterLocation, m_TileSize);
	
			FVector2D adjustedTileCount;	
			adjustedTileCount.X = m_TileCount.X -  (UUtilityLibrary::IsFloatEven(m_TileCount.X) ? 0.0f : 1.0f);
			adjustedTileCount.Y = m_TileCount.Y -  (UUtilityLibrary::IsFloatEven(m_TileCount.Y) ? 0.0f : 1.0f);

			// Calculate Grid Bottom Left Corner Location
			_bottomLeft = _center - (FVector(FVector2D(adjustedTileCount * 0.5f), 0.0f) * m_TileSize);
			break;
		}
		case EGridShape::Hexagon:
		{
			_center =  UUtilityLibrary::SnapVectorToVector(m_CenterLocation, m_TileSize * FVector(1.5f, 1.0f, 1.0f));
    
			FVector2D adjustedTileCount;	
			adjustedTileCount.X = m_TileCount.X / 3.0f * m_TileSize.X;
			adjustedTileCount.Y = m_TileCount.Y / 2.0f * m_TileSize.Y;			

			// Calculate Grid Bottom Left Corner Location
			_bottomLeft = _center - UUtilityLibrary::SnapVectorToVector(FVector(adjustedTileCount, 0.0f), m_TileSize * FVector(1.5f, 1.0f, 1.0f));
			break;
		}
		case EGridShape::Triangle:
		{
			_center =  UUtilityLibrary::SnapVectorToVector(m_CenterLocation, m_TileSize * FVector(2.0f, 1.0f, 1.0f));
    
            FVector2D adjustedTileCount;	
            adjustedTileCount.X = (m_TileCount.X - 1.0f) * 0.5f * m_TileSize.X;
            adjustedTileCount.Y = (m_TileCount.Y - 1.0f) * 0.25f * m_TileSize.Y;			

            // Calculate Grid Bottom Left Corner Location
            _bottomLeft = _center - UUtilityLibrary::SnapVectorToVector(FVector(adjustedTileCount, 0.0f), m_TileSize * FVector(2.0f, 1.0f, 1.0f));
			break;
		}
		default:
		{
			break;
		}
	}	
}
