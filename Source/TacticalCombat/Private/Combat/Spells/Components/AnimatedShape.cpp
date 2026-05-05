// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Spells/Components/AnimatedShape.h"

#include "Components/TimelineComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Shared/SharedConstants.h"

// Sets default values
AAnimatedShape::AAnimatedShape()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	m_SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	RootComponent = m_SceneComponent;
	
	m_AnimatedShapeComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AnimatedShape Component"));
	m_AnimatedShapeComponent->SetupAttachment(RootComponent);

	m_TimelineComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline Component"));


	static ConstructorHelpers::FObjectFinder<UCurveFloat> movementCurveObjFinder(TEXT("/Game/Resources/Spells/Curves/AC_SpellMovement.AC_SpellMovement"));
	if (movementCurveObjFinder.Succeeded())
	{
		m_MovementCurve = movementCurveObjFinder.Object;
	}
	static ConstructorHelpers::FObjectFinder<UCurveFloat> heighttCurveObjFinder(TEXT("/Game/Resources/Spells/Curves/AC_SpellHeight.AC_SpellHeight"));
	if (heighttCurveObjFinder.Succeeded())
	{
		m_HeightCurve = heighttCurveObjFinder.Object;
	}
	


}

void AAnimatedShape::BeginPlay()
{
	Super::BeginPlay();
}

void AAnimatedShape::PostSpawnInitialize(const FPostSpawnInitializeParam& _param)
{
	m_Mesh = _param.pMesh;
	m_Material = _param.pMaterial;
	m_Color =_param.Color;
	m_OriginTransform = _param.OriginTransform;
	m_TargetTransform = _param.TargetTransform;
	m_PlayRate = _param.PlayRate;

	_InitializeAnimatedShapeComponent();
	_InitializeTimelineComponent();
	m_TimelineComponent->PlayFromStart();
}

void AAnimatedShape::_InitializeAnimatedShapeComponent()
{
	m_AnimatedShapeComponent->SetStaticMesh(m_Mesh);
	m_AnimatedShapeComponent->SetMaterial(0, m_Material);
	m_AnimatedShapeComponent->SetVectorParameterValueOnMaterials("Color", FVector(m_Color));
}

void AAnimatedShape::_InitializeTimelineComponent()
{
	// 커브 적용
	FOnTimelineFloat movementUpdateEvent;
	movementUpdateEvent.BindUFunction(this, FName("_HandleMovementCurveUpdated"));
	m_TimelineComponent->AddInterpFloat(m_MovementCurve, movementUpdateEvent);
	
	FOnTimelineFloat heightUpdateEvent;
	heightUpdateEvent.BindUFunction(this, FName("_HandleHeightCurveUpdated"));
	m_TimelineComponent->AddInterpFloat(m_HeightCurve, heightUpdateEvent);

	FOnTimelineEvent TimelineFinishedEvent;
	TimelineFinishedEvent.BindUFunction(this, FName("_HandleTimelineFinished"));
	m_TimelineComponent->SetTimelineFinishedFunc(TimelineFinishedEvent);

	m_TimelineComponent->SetPlayRate(m_PlayRate);
}

#pragma region Timeline Update Handler
void AAnimatedShape::_HandleMovementCurveUpdated(float _value)
{
	FTransform adjustedTransform = UKismetMathLibrary::TLerp(m_OriginTransform, m_TargetTransform, _value);
	adjustedTransform.SetLocation(adjustedTransform.GetLocation() - FVector(0.0f, 0.0f, Tile::DEFAULT_TILE_SIZE_Z));
	SetActorTransform(adjustedTransform);
}

inline void AAnimatedShape::_HandleHeightCurveUpdated(float _value)
{
	FVector currentLocation = GetActorLocation();
	currentLocation.Z = _value;
	SetActorLocation(currentLocation);
}

inline void AAnimatedShape::_HandleTimelineFinished()
{
	FTimerHandle hTimer;
	GetWorld()->GetTimerManager().SetTimer(hTimer,  [this]() {
		Destroy();
		BroadcastAnimatedShapeFinished();
	}, 0.5f, false);
}



#pragma endregion