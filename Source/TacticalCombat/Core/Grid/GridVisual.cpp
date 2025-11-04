// Fill out your copyright notice in the Description page of Project Settings.


#include "GridVisual.h"

#include "Grid.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "TacticalCombat/Misc/Defines.h"

// Sets default values
AGridVisual::AGridVisual()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	m_SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = m_SceneComponent;

	m_InstancedStaticMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstancedStaticMeshComponent"));
	m_InstancedStaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	m_InstancedStaticMeshComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	m_InstancedStaticMeshComponent->SetCollisionResponseToChannel(GTC_Grid, ECollisionResponse::ECR_Block);
	m_InstancedStaticMeshComponent->NumCustomDataFloats = 4;
	RootComponent = m_InstancedStaticMeshComponent;
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
	m_InstancedStaticMeshComponent->SetStaticMesh(_pMesh);
	m_InstancedStaticMeshComponent->SetMaterial(0, _pMaterial);
	m_InstancedStaticMeshComponent->SetColorParameterValueOnMaterials("Color", _color);
	m_InstancedStaticMeshComponent->SetCollisionEnabled(_collisionEnabled);
}

void AGridVisual::AddInstance(const FIntPoint& _index, const FTransform& _transform, uint8 _tileStateMask)
{
	RemoveInstance(_index);
	m_InstancedStaticMeshComponent->AddInstance(_transform, false);
	m_InstanceIndices.Add(_index);
	
	FColor tileColor = _GetColorFromState(_tileStateMask);
	const uint8 index = m_InstanceIndices.Num() - 1;
	m_InstancedStaticMeshComponent->SetCustomDataValue(index, 0, tileColor.R);
	m_InstancedStaticMeshComponent->SetCustomDataValue(index, 1, tileColor.G);
	m_InstancedStaticMeshComponent->SetCustomDataValue(index, 2, tileColor.B);
	m_InstancedStaticMeshComponent->SetCustomDataValue(index, 3, tileColor.A);
}

void AGridVisual::RemoveInstance(const FIntPoint& _index)
{
	if (m_InstanceIndices.Contains(_index))
	{
		int32 foundInstanceIndex = m_InstanceIndices.Find(_index);
		m_InstancedStaticMeshComponent->RemoveInstance(foundInstanceIndex);
		m_InstanceIndices.Remove(_index);
	}
}

void AGridVisual::ClearInstances()
{
	m_InstancedStaticMeshComponent->ClearInstances();
	m_InstanceIndices.Empty();
}

FColor AGridVisual::_GetColorFromState(uint8 _tileStateMask)
{
	bool isSelected = (_tileStateMask & static_cast<uint8>(ETileStateFlags::Selected)) != 0;
	if (isSelected)
	{
		return FColor::Red;
	}

	bool isHovered = (_tileStateMask & static_cast<uint8>(ETileStateFlags::Hovered)) != 0;
	if (isHovered)
	{
		return FColor::Yellow;
	}

	bool isNeighbor = (_tileStateMask & static_cast<uint8>(ETileStateFlags::Neighbor)) != 0;
	if (isNeighbor)
	{
		return FColor::Purple;
	}

	FColor color = FColor::Black;
	color.A = 0.0f;
	return color;
}
