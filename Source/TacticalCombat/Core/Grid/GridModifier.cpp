// Fill out your copyright notice in the Description page of Project Settings.


#include "GridModifier.h"

#include "TacticalCombat/Libraries/GridLibrary.h"
#include "TacticalCombat/Misc/Defines.h"
#include "TacticalCombat/Misc/Enums.h"
#include "TacticalCombat/Structure/GridShapeData.h"

TMap<ETileType, FColor> AGridModifier::TileColorMap = {
	{ETileType::None, FColor::Black},
	{ETileType::Normal, FColor::White},
	{ETileType::Obstacle, FColor::Red}
};


// Sets default values
AGridModifier::AGridModifier()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	SetActorHiddenInGame(true);
	
	m_SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	RootComponent = m_SceneComponent;
	
	m_StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh Component"));
	m_StaticMeshComponent->SetupAttachment(RootComponent);
	m_StaticMeshComponent->SetCollisionResponseToChannel(GTC_GroundAndGridModifier, ECollisionResponse::ECR_Overlap);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> meshObjectFinder(TEXT("/Game/Grids/GridShapes/Square/SM_Grid_Square.SM_Grid_Square"));
	if (meshObjectFinder.Succeeded())
	{
		m_StaticMeshComponent->SetStaticMesh(meshObjectFinder.Object);
	}	

	static ConstructorHelpers::FObjectFinder<UMaterial> materialObjectFinder(TEXT("/Game/Grids/GridShapes/M_Grid_Flat_Filled.M_Grid_Flat_Filled"));
	if (materialObjectFinder.Succeeded())
	{
		m_StaticMeshComponent->SetMaterial(0, materialObjectFinder.Object);	
	}
}

void AGridModifier::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	FGridShapeData gridShapeData = UGridLibrary::GetGridShape(m_Shape);
	m_StaticMeshComponent->SetStaticMesh(gridShapeData.Mesh.Get());
	m_StaticMeshComponent->SetMaterial(0, gridShapeData.FlatMaterial);
	
	m_StaticMeshComponent->SetScalarParameterValueOnMaterials(TEXT("IsFilled"), 1.0f);
	m_StaticMeshComponent->SetColorParameterValueOnMaterials(TEXT("Color"), TileColorMap[m_Type]);	
}

// Called when the game starts or when spawned
void AGridModifier::BeginPlay()
{
	Super::BeginPlay();
	
}

