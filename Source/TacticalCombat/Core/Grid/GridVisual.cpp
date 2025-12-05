// Fill out your copyright notice in the Description page of Project Settings.


#include "GridVisual.h"

#include "Grid.h"
#include "GridInstancedStaticMeshComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "TacticalCombat/Misc/Defines.h"

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

// Called every frame
void AGridVisual::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGridVisual::InitializeGridVisual(AGrid* _pGrid)
{
	FGridShapeData gridShapeData = _pGrid->GetGridShapeData();
	InitializeGridMeshInst(gridShapeData.FlatMesh, gridShapeData.FlatMaterial, FColor(0.0f, 0.0f, 0.0f, 0.5f), ECollisionEnabled::Type::QueryOnly);
	SetActorLocation(FVector::ZeroVector);
	SetZOffset(m_ZOffset);	
}

void AGridVisual::SetZOffset(float _offset)
{
	m_ZOffset = _offset;

	FVector visualLocation = GetActorLocation();
	visualLocation.Z = m_ZOffset;
	SetActorLocation(visualLocation);	
}

void AGridVisual::DestroyGridVisual()
{
	ClearInstances();
}

void AGridVisual::UpdateTileVisual(const FTileData& _tileData)
{
	RemoveInstance(_tileData.Index);
	if (AGrid::IsWalkableTile(_tileData.Type))
	{
		AddInstance(_tileData.Index, _tileData.Transform, _tileData.StateMask);
	}
}

void AGridVisual::InitializeGridMeshInst(UStaticMesh* const _pMesh, UMaterialInstance* const _pMaterial, const FColor& _color, ECollisionEnabled::Type _collisionEnabled)
{
	m_GridInstancedStaticMeshComponent->SetStaticMesh(_pMesh);
	m_GridInstancedStaticMeshComponent->SetMaterial(0, _pMaterial);
	m_GridInstancedStaticMeshComponent->SetColorParameterValueOnMaterials("Color", _color);
	m_GridInstancedStaticMeshComponent->SetCollisionEnabled(_collisionEnabled);
}

void AGridVisual::AddInstance(const FIntPoint& _index, const FTransform& _transform, uint8 _tileStateMask)
{
	m_GridInstancedStaticMeshComponent->AddInstance(_index, _transform, _tileStateMask);
}

void AGridVisual::RemoveInstance(const FIntPoint& _index)
{
	m_GridInstancedStaticMeshComponent->RemoveInstance(_index);
}

void AGridVisual::ClearInstances()
{
	m_GridInstancedStaticMeshComponent->ClearInstances();
}
