// Fill out your copyright notice in the Description page of Project Settings.


#include "Unit/Unit.h"

#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "Library/UnitsLibrary.h"
#include "UI/DebugMenu/Elements/Button/UnitButton.h"

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


void AUnit::OnConstruction(const FTransform& _transform)
{
#if WITH_EDITOR
	UWorld* pWorld = GetWorld();
	if (pWorld && pWorld->WorldType == EWorldType::Editor) // 에디터에서 편집중일때에만 실행
	{
		InitializeUnit(m_UnitType);
	}
#endif
}

// Called when the game starts or when spawned
void AUnit::BeginPlay()
{
	Super::BeginPlay();
	
}

void AUnit::InitializeUnit(ETacticalUnitType _type)
{
	m_UnitType = _type;
	FUnitAssetsTableRow assetTableRow = UUnitsLibrary::GetDefaultUnitAssetsDataFromUnitType(m_UnitType);
	m_SkeletalMeshComponent->SetSkeletalMesh(assetTableRow.Assets.Mesh);
	m_SkeletalMeshComponent->SetAnimInstanceClass(assetTableRow.Assets.AnimationBPClass);
	if (m_UnitType == ETacticalUnitType::Ranger)
	{
		m_SkeletalMeshComponent->SetRelativeScale3D(FVector(0.39, 0.39, 0.39));
	}
}


