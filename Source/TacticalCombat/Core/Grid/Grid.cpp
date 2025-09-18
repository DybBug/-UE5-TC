// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "TacticalCombat/Libraries/UtilityLibrary.h"
#include "TacticalCombat/Structure/GridShapeData.h"

// Sets default values
AGrid::AGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UDataTable> dataTableObjectFinder(TEXT("/Game/Girds/GridShapes/DT_GridShape"));
	if (dataTableObjectFinder.Succeeded())
	{
		m_GridShapeDataTable = dataTableObjectFinder.Object;
	}	

	m_SceneComp = CreateDefaultSubobject<USceneComponent>("Scene Component");
	RootComponent = m_SceneComp;

	m_InstancedStaticMeshComp = CreateDefaultSubobject<UInstancedStaticMeshComponent>("Instanced StaticMesh Component");
	m_InstancedStaticMeshComp->SetupAttachment(m_SceneComp);
}

void AGrid::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	SpawnGrid(GetActorLocation(), m_TileSize, m_TileCount, m_Shape);
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

void AGrid::SpawnGrid(const FVector& _centerLocation, const FVector& _tileSize, const FVector2D& _tileCount, EGridShape _shape)
{
	m_CenterLocation = _centerLocation;
	m_TileSize = _tileSize;
	m_Shape = _shape;
	m_TileCount.X = FMath::RoundToInt32(_tileCount.X);
	m_TileCount.Y = FMath::RoundToInt32(_tileCount.Y);
	
	DestroyGrid();
	
	if (m_GridShapeDataTable)
	{
		FName shapeName = *StaticEnum<EGridShape>()->GetNameStringByValue((uint8)m_Shape);
		const FGridShapeData* const pGridShapeData = m_GridShapeDataTable->FindRow<FGridShapeData>(shapeName, TEXT("Data Table Lookup"));
		if (pGridShapeData == nullptr)
			return;
		
		m_InstancedStaticMeshComp->SetStaticMesh(pGridShapeData->FlatMesh);
		m_InstancedStaticMeshComp->SetMaterial(0, pGridShapeData->FlatBoardMaterial);
		
		CalculateCenterAndBottomLeft(m_CenterLocation, m_GridBottomLeftCornerLocation);

		// Make Tiles
		int32 rowCount = FMath::RoundToInt(m_TileCount.X);
		for (int32 row = 0; row < rowCount; ++row)
		{
			if (m_Shape == EGridShape::Hexagon)
			{
				int32 startCol = UUtilityLibrary::IsIntEven(row) ? 0 : 1;
				int32 colCount = FMath::RoundToInt(m_TileCount.Y) * 2;
				for (int32 col = startCol; col < colCount; col += 2)
				{			
					FTransform tileTransform;
					tileTransform.SetLocation(_GetTileLocationFromGridIndex(row, col));
					tileTransform.SetRotation(_GetTileRotationFromGridIndex(row, col).Quaternion());
					tileTransform.SetScale3D(m_TileSize / pGridShapeData->MeshSize);

					m_InstancedStaticMeshComp->AddInstance(tileTransform, true);
				}
			}
			else
			{
				int32 colCount = FMath::RoundToInt(m_TileCount.Y);
				for (int32 col = 0; col < colCount; ++col)
				{			
					FTransform tileTransform;
					tileTransform.SetLocation(_GetTileLocationFromGridIndex(row, col));
					tileTransform.SetRotation(_GetTileRotationFromGridIndex(row, col).Quaternion());
					tileTransform.SetScale3D(m_TileSize / pGridShapeData->MeshSize);

					m_InstancedStaticMeshComp->AddInstance(tileTransform, true);
				}
			}

		}
	}
}

void AGrid::DestroyGrid()
{
	m_InstancedStaticMeshComp->ClearInstances();
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

	static const float zOffset = 2.0f;
	FVector adjustedTileSize =  m_TileSize * sizeOffset;
	return m_GridBottomLeftCornerLocation + FVector(adjustedTileSize.X * _row, adjustedTileSize.Y * _col, zOffset);
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


