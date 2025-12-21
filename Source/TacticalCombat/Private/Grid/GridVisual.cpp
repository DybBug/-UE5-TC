// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid/GridVisual.h"

#include "Grid/Grid.h"
#include "Grid/GridInstancedStaticMeshComponent.h"
#include "Shared/SharedDefines.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGridVisual::AGridVisual()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	m_SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = m_SceneComponent;

	m_GridInstancedStaticMeshComponent = CreateDefaultSubobject<UGridInstancedStaticMeshComponent>(TEXT("GridInstancedStaticMeshComponent"));
	m_GridInstancedStaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	m_GridInstancedStaticMeshComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	m_GridInstancedStaticMeshComponent->SetCollisionResponseToChannel(GTC_Grid, ECollisionResponse::ECR_Block);
	m_GridInstancedStaticMeshComponent->NumCustomDataFloats = 4;
	RootComponent = m_GridInstancedStaticMeshComponent;

	m_TacticalGridInstancedStaticMeshComponent = CreateDefaultSubobject<UGridInstancedStaticMeshComponent>(TEXT("TacticalGridInstancedStaticMeshComponent"));
	m_TacticalGridInstancedStaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	m_TacticalGridInstancedStaticMeshComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	m_TacticalGridInstancedStaticMeshComponent->SetCollisionResponseToChannel(GTC_Grid, ECollisionResponse::ECR_Block);
	m_TacticalGridInstancedStaticMeshComponent->NumCustomDataFloats = 4;
	RootComponent = m_TacticalGridInstancedStaticMeshComponent;
	
	SetActorLocation(FVector::ZeroVector);
}

void AGridVisual::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}


// Called when the game starts or when spawned
void AGridVisual::BeginPlay()
{
	Super::BeginPlay();
}

void AGridVisual::InitializeGridVisual(AGrid* _pGrid)
{
	m_Grid = _pGrid;
	FGridShapeTableRow gridShapeData = m_Grid->GetGridShapeData();
	m_GridInstancedStaticMeshComponent->InitializeGridMeshInst(gridShapeData.FlatMesh, gridShapeData.FlatMaterial, false, ECollisionEnabled::Type::QueryOnly);
	m_TacticalGridInstancedStaticMeshComponent->InitializeGridMeshInst(gridShapeData.Mesh, gridShapeData.MeshMaterial, true, ECollisionEnabled::Type::QueryOnly);
	SetActorLocation(FVector::ZeroVector);
	SetZOffset(m_ZOffset);
	_SetGridLowerZ(DEFAULT_GRID_LOWER_Z);
	SetIsTactical(m_bIsTactical);
}

void AGridVisual::DestroyGridVisual()
{
	m_GridInstancedStaticMeshComponent->ClearInstances();
	m_TacticalGridInstancedStaticMeshComponent->ClearInstances();
	_SetGridLowerZ(DEFAULT_GRID_LOWER_Z);
}

void AGridVisual::UpdateTileVisual(const FTileData& _tileData)
{
	m_GridInstancedStaticMeshComponent->RemoveInstance(_tileData.Index);
	if (AGrid::IsWalkableTile(_tileData.Type))
	{
		m_GridInstancedStaticMeshComponent->AddInstance(_tileData.Index, _tileData.Transform, _tileData.StateMask, _tileData.Type);
	}
	
	_UpdateTileVisualTactical(_tileData);
}

void AGridVisual::_UpdateTileVisualTactical(const FTileData& _tileData)
{
	if (_tileData.Transform.GetTranslation().Z < m_GridLowerZ)
	{
		_SetGridLowerZ(_tileData.Transform.GetTranslation().Z);
	}

	if (!m_bIsTactical)
	{
		m_bIsNeedToReGenerateTacticalOnNextEnable = true;
		return;
	}
	
	m_TacticalGridInstancedStaticMeshComponent->RemoveInstance(_tileData.Index);
	if (_tileData.Type != ETileType::None)
	{
		FTransform tileTransform = _tileData.Transform;
		FVector tileScale = tileTransform.GetScale3D();
		tileScale .Z = (tileTransform.GetTranslation().Z - m_GridLowerZ) / m_Grid->GetGridShapeData().MeshSize.Z;
		tileTransform.SetScale3D(tileScale);
		m_TacticalGridInstancedStaticMeshComponent->AddInstance(_tileData.Index, tileTransform, _tileData.StateMask, _tileData.Type);
	}
}

void AGridVisual::_SetGridLowerZ(int32 _z)
{
	m_GridLowerZ = _z;
	m_bIsNeedToReGenerateTacticalOnNextEnable = true;
}

void AGridVisual::SetZOffset(float _offset)
{
	m_ZOffset = _offset;

	m_GridInstancedStaticMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, m_ZOffset));
	FVector visualLocation = GetActorLocation();
	visualLocation.Z = m_ZOffset;
	SetActorLocation(visualLocation);	
}

void AGridVisual::SetIsTactical(bool _bIsTactical)
{
	m_bIsTactical = _bIsTactical;
	m_TacticalGridInstancedStaticMeshComponent->SetHiddenInGame(!m_bIsTactical);

	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("NotTactical"), actors);
	for (AActor* pActor : actors)
	{
		pActor->SetActorHiddenInGame(m_bIsTactical);
	}

	if (m_bIsTactical && m_bIsNeedToReGenerateTacticalOnNextEnable)
	{
		m_bIsNeedToReGenerateTacticalOnNextEnable = false;
		for (const auto& it : m_Grid->GetGridTileMap())
		{
			UpdateTileVisual(it.Value);
		}
	}
}