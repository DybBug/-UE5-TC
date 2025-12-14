// Fill out your copyright notice in the Description page of Project Settings.


#include "Unit/Unit.h"

#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "Engine/SkeletalMesh.h"
#include "Shared/SharedEnums.h"

// Sets default values
AUnit::AUnit()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	m_SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	RootComponent = m_SceneComponent;

	m_SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh Component"));
	m_SkeletalMeshComponent->SetupAttachment(RootComponent);
}

void AUnit::OnConstruction(const FTransform& Transform)
{
	if (!m_UnitTypesToSkeletalMesh.IsEmpty())
	{
		m_SkeletalMeshComponent->SetSkeletalMesh(m_UnitTypesToSkeletalMesh[m_UnitType]);
	}

	if (!m_UnitTypesToAnimInstanceClass.IsEmpty())
	{
		m_SkeletalMeshComponent->SetAnimInstanceClass(m_UnitTypesToAnimInstanceClass[m_UnitType]);
	}
}

// Called when the game starts or when spawned
void AUnit::BeginPlay()
{
	Super::BeginPlay();
	
}


