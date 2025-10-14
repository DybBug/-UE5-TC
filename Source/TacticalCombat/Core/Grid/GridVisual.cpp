// Fill out your copyright notice in the Description page of Project Settings.


#include "GridVisual.h"

#include "Grid.h"
#include "GridMeshInst.h"

// Sets default values
AGridVisual::AGridVisual()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	m_SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = m_SceneComponent;

	m_ChildActorGridMeshInst = CreateDefaultSubobject<UChildActorComponent>(TEXT("ChildActorComponent"));
	m_ChildActorGridMeshInst->SetChildActorClass(AGridMeshInst::StaticClass());
	m_ChildActorGridMeshInst->SetupAttachment(RootComponent);

	// static ConstructorHelpers::FClassFinder<AActor> gridMeshInstClassFinder(TEXT("/Game/Grids/BP_GridMeshInst.BP_GridMeshInst_C"));
	// if (gridMeshInstClassFinder.Succeeded())
	// {
	// 	m_ChildActorGridMeshInst->SetChildActorClass(gridMeshInstClassFinder.Class);
	// }
}

// Called when the game starts or when spawned
void AGridVisual::BeginPlay()
{
	Super::BeginPlay();
}

void AGridVisual::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	m_GridMeshInst = Cast<AGridMeshInst>(m_ChildActorGridMeshInst->GetChildActor());
}


// Called every frame
void AGridVisual::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGridVisual::InitializeGridVisual(AGrid* _pGrid)
{
	FGridShapeData gridShapeData = _pGrid->GetGridShapeData();
	m_GridMeshInst->InitializeGridMeshInst(gridShapeData.FlatMesh, gridShapeData.FlatMaterial, FColor(0.0f, 0.0f, 0.0f, 0.5f), ECollisionEnabled::Type::QueryOnly);
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
	m_GridMeshInst->ClearInstances();
}

void AGridVisual::UpdateTileVisual(const FTileData& _tileData)
{
	m_GridMeshInst->RemoveInstance(_tileData.Index);
	if (AGrid::IsWalkableTile(_tileData.Type))
	{
		m_GridMeshInst->AddInstance(_tileData.Index, _tileData.Transform, _tileData.StateMask);
	}
}

