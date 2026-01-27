// Fill out your copyright notice in the Description page of Project Settings.


#include "Unit/Unit.h"

#include "VectorUtil.h"
#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/UnitAnimInstance.h"
#include "Components/TimelineComponent.h"
#include "Grid/Grid.h"
#include "Kismet/KismetMathLibrary.h"
#include "Library/UnitsLibrary.h"
#include "Shared/SharedDefines.h"
#include "UI/DebugMenu/Elements/Button/UnitButton.h"

AUnit* AUnit::Spawn(UWorld* _pWorld,  ETacticalUnitType _type, AGrid* _pGrid)
{
	check(_pWorld);
	AUnit* newUnit = _pWorld->SpawnActor<AUnit>(AUnit::StaticClass());
	newUnit->InitializeUnit(_type, _pGrid);
	return newUnit;
}

// Sets default values
AUnit::AUnit()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	m_SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	RootComponent = m_SceneComponent;

	m_SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh Component"));
	m_SkeletalMeshComponent->SetRelativeRotation(FRotator(0, -90, 0));
	m_SkeletalMeshComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	m_SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	m_SkeletalMeshComponent->SetCollisionResponseToChannel(GTC_Unit, ECollisionResponse::ECR_Block);
	m_SkeletalMeshComponent->SetupAttachment(RootComponent);

	m_UnitMovementTimelineComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("Unit Movement Timeline Component"));

	static ConstructorHelpers::FObjectFinder<UCurveFloat> unitLocationCurve(TEXT("/Game/Resources/Units/Curves/AC_UnitLocation.AC_UnitLocation"));
	if (unitLocationCurve.Succeeded())
	{
		m_UnitLocationCurve = unitLocationCurve.Object;
	}

	static ConstructorHelpers::FObjectFinder<UCurveFloat> unitRotationCurve(TEXT("/Game/Resources/Units/Curves/AC_UnitRotation.AC_UnitRotation"));
	if (unitRotationCurve.Succeeded())
	{
		m_UnitRotationCurve = unitRotationCurve.Object;		
	}

	static ConstructorHelpers::FObjectFinder<UCurveFloat> unitJumpCurve(TEXT("/Game/Resources/Units/Curves/AC_UnitJump.AC_UnitJump"));
	if (unitJumpCurve.Succeeded())
	{
		m_UnitJumpCurve = unitJumpCurve.Object;		
	}
}

void AUnit::OnConstruction(const FTransform& _transform)
{
#if WITH_EDITOR
	UWorld* pWorld = GetWorld();
	if (pWorld && pWorld->WorldType == EWorldType::Editor) // 에디터에서 편집중일때에만 실행
	{
		InitializeUnit(m_UnitType, nullptr);
	}
#endif
}

// Called when the game starts or when spawned
void AUnit::BeginPlay()
{
	Super::BeginPlay();

	FOnTimelineFloat locationUpdateCallback;
	locationUpdateCallback.BindUFunction(this, FName("_HandleUnitLocationCurveUpdated"));
	m_UnitMovementTimelineComponent->AddInterpFloat(m_UnitLocationCurve, locationUpdateCallback);

	FOnTimelineFloat rotationUpdateCallback;
	rotationUpdateCallback.BindUFunction(this, FName("_HandleUnitRotationCurveUpdated"));
	m_UnitMovementTimelineComponent->AddInterpFloat(m_UnitRotationCurve, rotationUpdateCallback);

	FOnTimelineFloat jumpUpdateCallback;
	jumpUpdateCallback.BindUFunction(this, FName("_HandleUnitJumpCurveUpdated"));
	m_UnitMovementTimelineComponent->AddInterpFloat(m_UnitJumpCurve, jumpUpdateCallback);

	FOnTimelineEvent finishCallback;
	finishCallback.BindUFunction(this, FName("_HandleUnitMovementTimelineFinishedWithNotify"));
	m_UnitMovementTimelineComponent->SetTimelineFinishedFunc(finishCallback);
}

void AUnit::InitializeUnit(ETacticalUnitType _type, AGrid* _pGrid)
{
	m_Grid = _pGrid;
	m_UnitType = _type;
	if (m_UnitType == ETacticalUnitType::Ranger)
	{
		m_SkeletalMeshComponent->SetRelativeScale3D(FVector(0.39, 0.39, 0.39));
	}
	
	FUnitTableRow assetTableRow = UUnitsLibrary::GetDefaultUnitAssetsDataFromUnitType(m_UnitType);
	m_UnitData = assetTableRow;
	m_SkeletalMeshComponent->SetSkeletalMesh(assetTableRow.Assets.Mesh);
	m_SkeletalMeshComponent->SetAnimInstanceClass(assetTableRow.Assets.AnimationBPClass);
	
	_SetAnimationState(EUnitAnimationState::Idle);
}

void AUnit::UpdateVisual()
{
	FVector colorMultiply = m_bIsSelected ? FVector(0.1f, 0.5f, 1.0f) : FVector(1.0f, 1.0f, 1.0f);
	colorMultiply *= m_bIsHovered ? 2.5f : 1.0f;
	colorMultiply *= m_bIsSelected ? 2.0f : 1.0f;
	m_SkeletalMeshComponent->SetVectorParameterValueOnMaterials(TEXT("ColorMultiply"), colorMultiply);
}

void AUnit::FollowPathWithNotify(const TArray<FIntPoint>& _path)
{
	if (_path.Num() <= 0)
	{
		_SetAnimationState(EUnitAnimationState::Idle);
		if (OnUnitFinishedWalking.IsBound())
		{
			OnUnitFinishedWalking.Broadcast(this);
		}
		return;
	}

	m_CurrentPathToFollow = _path;

	// 걷기 시작 이벤트 호출
	if (OnUnitStartedWalking.IsBound())
	{
		OnUnitStartedWalking.Broadcast(this);
	}

	// 걷기 시작
	_SetAnimationState(EUnitAnimationState::Walk);

	m_PreviousTransform = GetActorTransform();

	FIntPoint pathPoint = m_CurrentPathToFollow[0];
	FTransform foundedNextTransform = m_Grid->GetGridTileMap().Find(pathPoint)->Transform;
	m_NextTransform.SetLocation(foundedNextTransform.GetLocation());
	FRotator nextRotation = m_NextTransform.GetRotation().Rotator();
	nextRotation.Yaw = UKismetMathLibrary::FindLookAtRotation(
			m_PreviousTransform.GetLocation(),
			m_NextTransform.GetLocation()).Yaw;
	
	m_NextTransform.SetRotation(nextRotation.Quaternion());

	float playRate = m_UnitMovementTimelineComponent->GetTimelineLength() / m_MoveDurationPerTile;
	m_UnitMovementTimelineComponent->SetPlayRate(playRate);
	m_UnitMovementTimelineComponent->PlayFromStart();
}

void AUnit::SetIsHovered(bool _bIsHovered)
{
	m_bIsHovered = _bIsHovered;
	UpdateVisual();
}

void AUnit::SetIsSelected(bool _bIsSelected)
{
	m_bIsSelected = _bIsSelected;
	UpdateVisual();
}

void AUnit::_SetAnimationState(EUnitAnimationState _state)
{
	// 1. 애니메이션 인스턴스를 가져옵니다. (UObject 타입으로 충분함)
	UAnimInstance* pAnimInst = m_SkeletalMeshComponent->GetAnimInstance();

	// 2. 인터페이스를 구현하고 있는지 확인합니다.
	if (pAnimInst && pAnimInst->Implements<UUnitAnimationInterface>())
	{
		// 3. Execute_를 호출할 때는 '인터페이스 포인터'가 아니라 '오브젝트 포인터'를 넘깁니다.
		IUnitAnimationInterface::Execute_SetUnitAnimationState(pAnimInst, _state);
	}
}

#pragma region Timeline Event Handles
void AUnit::_HandleUnitLocationCurveUpdated(float _value)
{	
	FVector currLocation = UKismetMathLibrary::VLerp(m_PreviousTransform.GetLocation(), m_NextTransform.GetLocation(), _value);	
	SetActorLocation(currLocation);
}

void AUnit::_HandleUnitRotationCurveUpdated(float _value)
{
	FRotator currRotator =	UKismetMathLibrary::RLerp(m_PreviousTransform.GetRotation().Rotator(), m_NextTransform.GetRotation().Rotator(), _value, true);
	SetActorRotation(currRotator);
}

void AUnit::_HandleUnitJumpCurveUpdated(float _value)
{
	FVector currLocation = GetActorLocation();
	bool isNearlyZ = FMath::Abs(m_PreviousTransform.GetLocation().Z - m_NextTransform.GetLocation().Z) < 5.0f;
	currLocation.Z += isNearlyZ ? 0.0f : _value;
	SetActorLocation(currLocation);
}

void AUnit::_HandleUnitMovementTimelineFinishedWithNotify()
{
	if (OnUnitResearchedNewTile.IsBound())
	{
		OnUnitResearchedNewTile.Broadcast(this, m_CurrentPathToFollow[0]);
	}
	m_CurrentPathToFollow.RemoveAt(0);
	FollowPathWithNotify(m_CurrentPathToFollow);
}

#pragma endregion


