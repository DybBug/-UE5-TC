// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid.h"

#include "GridModifier.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TacticalCombat/Libraries/GridLibrary.h"
#include "TacticalCombat/Libraries/UtilityLibrary.h"
#include "TacticalCombat/Misc/Defines.h"
#include "TacticalCombat/Structure/GridShapeData.h"
#include "TacticalCombat/Misc/Enums.h"

// Sets default values
AGrid::AGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	m_SceneComponent = CreateDefaultSubobject<USceneComponent>("Scene Component");
	RootComponent = m_SceneComponent;

	m_InstancedStaticMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>("Instanced StaticMesh Component");
	m_InstancedStaticMeshComponent->SetupAttachment(m_SceneComponent);
}

void AGrid::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	SpawnGrid(m_InstancedStaticMeshComponent->GetComponentLocation(), m_TileSize, m_TileCount, m_Shape);
}

// Called when the game starts or when spawned
void AGrid::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGrid::Tick(float _deltaTime)
{
	Super::Tick(_deltaTime);
}

void AGrid::SpawnGrid(const FVector& _centerLocation, const FVector& _tileSize, const FIntPoint& _tileCount, EGridShape _shape, bool _bIsUseEnvironment)
{
	m_CenterLocation = _centerLocation;
	m_TileSize = _tileSize;
	m_Shape = _shape;
	m_TileCount = _tileCount;
	
	DestroyGrid();

	FGridShapeData girdShapeData = UGridLibrary::GetGridShape(m_Shape);		
	m_InstancedStaticMeshComponent->SetStaticMesh(girdShapeData.FlatMesh);
	m_InstancedStaticMeshComponent->SetMaterial(0, girdShapeData.FlatBoardMaterial);
	
	SetZOffset(m_ZOffset);
	CalculateCenterAndBottomLeft(m_CenterLocation, m_GridBottomLeftCornerLocation);

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
			tileTransform.SetLocation(_GetTileLocationFromGridIndex(row, col));
			tileTransform.SetRotation(_GetTileRotationFromGridIndex(row, col).Quaternion());
			tileTransform.SetScale3D(m_TileSize / girdShapeData.MeshSize);

			if (_bIsUseEnvironment)
			{					
				ETileType tracedTileType;
				FVector hitLocation = TraceForGround(tileTransform.GetLocation(), tracedTileType);					
				if (!IsWalkableTile(tracedTileType))
					continue;
				
				tileTransform.SetLocation(hitLocation);
			}
			
			m_InstancedStaticMeshComponent->AddInstance(tileTransform, false);				
		}
	}
	
}

void AGrid::DestroyGrid()
{
	m_InstancedStaticMeshComponent->ClearInstances();
}

void AGrid::SetZOffset(float _zOffset)
{
	m_ZOffset = _zOffset;
	FVector instancedStaticMeshCompLoc = GetActorLocation();
	instancedStaticMeshCompLoc.Z = m_ZOffset;
	SetActorLocation(instancedStaticMeshCompLoc);
}

void AGrid::CalculateCenterAndBottomLeft(FVector& _center, FVector& _bottomLeft)
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
	if (hitResults.Num() > 0)
	{
		_hitTileType = ETileType::Normal;

		FVector resultLocation = _location;
		for (const FHitResult hitResult : hitResults)
		{			
			if (AGridModifier* pGridModifier = Cast<AGridModifier>(hitResult.GetActor()))
			{
				_hitTileType = pGridModifier->GetType();
			}
			else
			{
				resultLocation.Z = FMath::GridSnap(hitResult.ImpactPoint.Z, m_TileSize.Z);
			}
		}
		
		return resultLocation;
	}
	
	_hitTileType = ETileType::None;
	return FVector(-99999, -99999, -99999);
}

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

FVector AGrid::_GetTileLocationFromGridIndex(int _row, int _col)
{
	FVector sizeOffset = FVector::Zero();
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
			sizeOffset = FVector::Zero();
			break;
		}
	}
	
	FVector adjustedTileSize =  m_TileSize * sizeOffset;
	return m_GridBottomLeftCornerLocation + FVector(adjustedTileSize.X * _row, adjustedTileSize.Y * _col, adjustedTileSize.Z);
}

FRotator AGrid::_GetTileRotationFromGridIndex(int _row, int _col)
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


