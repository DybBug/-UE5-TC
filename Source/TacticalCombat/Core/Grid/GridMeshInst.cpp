// Fill out your copyright notice in the Description page of Project Settings.


#include "GridMeshInst.h"

#include "Grid.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "TacticalCombat/Misc/Defines.h"
#include "TacticalCombat/Misc/Enums.h"


// Sets default values
AGridMeshInst::AGridMeshInst()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	m_InstancedStaticMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstancedStaticMeshComponent"));
	m_InstancedStaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	m_InstancedStaticMeshComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	m_InstancedStaticMeshComponent->SetCollisionResponseToChannel(GTC_Grid, ECollisionResponse::ECR_Block);
	m_InstancedStaticMeshComponent->NumCustomDataFloats = 4;
	RootComponent = m_InstancedStaticMeshComponent;
	SetActorLocation(FVector::ZeroVector);
}


// Called when the game starts or when spawned
void AGridMeshInst::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGridMeshInst::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

// Called every frame
void AGridMeshInst::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGridMeshInst::InitializeGridMeshInst(UStaticMesh* const _pMesh, UMaterialInstance* const _pMaterial, const FColor& _color, ECollisionEnabled::Type _collisionEnabled)
{
	m_InstancedStaticMeshComponent->SetStaticMesh(_pMesh);
	m_InstancedStaticMeshComponent->SetMaterial(0, _pMaterial);
	m_InstancedStaticMeshComponent->SetColorParameterValueOnMaterials("Color", _color);
	m_InstancedStaticMeshComponent->SetCollisionEnabled(_collisionEnabled);
}

void AGridMeshInst::AddInstance(const FIntPoint& _index, const FTransform& _transform, uint8 _tileStateMask)
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

void AGridMeshInst::RemoveInstance(const FIntPoint& _index)
{
	if (m_InstanceIndices.Contains(_index))
	{
		int32 foundInstanceIndex = m_InstanceIndices.Find(_index);
		m_InstancedStaticMeshComponent->RemoveInstance(foundInstanceIndex);
		m_InstanceIndices.Remove(_index);
	}
}

void AGridMeshInst::ClearInstances()
{
	m_InstancedStaticMeshComponent->ClearInstances();
	m_InstanceIndices.Empty();
}

FColor AGridMeshInst::_GetColorFromState(uint8 _tileStateMask)
{
	bool isSelected = (_tileStateMask & static_cast<uint8>(ETileStateFlags::Selected)) != 0;
	if (isSelected)
	{
		return FColor::Orange;
	}

	bool isHovered = (_tileStateMask & static_cast<uint8>(ETileStateFlags::Hovered)) != 0;
	if (isHovered)
	{
		return FColor::Yellow;
	}

	FColor color = FColor::Black;
	color.A = 0.0f;
	return color;
}
