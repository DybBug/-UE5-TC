// Fill out your copyright notice in the Description page of Project Settings.


#include "TCPawn.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"


// Sets default values
ATCPawn::ATCPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

#pragma region Setup Components
	m_SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	RootComponent = m_SceneComp;
	
	m_SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm Component"));
	m_SpringArmComp->SetupAttachment(RootComponent);
	m_SpringArmComp->bDoCollisionTest = false;
	m_SpringArmComp->TargetArmLength = 700.0f;
	m_SpringArmComp->SetRelativeRotation(FRotator(-60.0f, 0.0f, 0.0f));
	
	m_CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	m_CameraComp->SetupAttachment(m_SpringArmComp);
#pragma endregion

#pragma region Setup Assets
	if (!m_DefaultInputMappingContext)
	{
		static ConstructorHelpers::FObjectFinder<UInputMappingContext> defaultInputMappingContextFinder(TEXT("/Game/Input/IMC_Default.IMC_Default"));
		if (defaultInputMappingContextFinder.Succeeded())
		{
			m_DefaultInputMappingContext = defaultInputMappingContextFinder.Object;
		}
	}

	if (!m_CameraMovingInputAction)
	{
		static ConstructorHelpers::FObjectFinder<UInputAction> cameraMovingInputActionFinder(TEXT("/Game/Input/IA_Camera_Moving.IA_Camera_Moving"));
		if (cameraMovingInputActionFinder.Succeeded())
		{
			m_CameraMovingInputAction = cameraMovingInputActionFinder.Object;
		}
	}

	if (!m_CameraRotationInputAction)
	{
		static ConstructorHelpers::FObjectFinder<UInputAction> cameraRotationInputActionFinder(TEXT("/Game/Input/IA_Camera_Rotation.IA_Camera_Rotation"));
		if (cameraRotationInputActionFinder.Succeeded())
		{
			m_CameraRotationInputAction = cameraRotationInputActionFinder.Object;
		}
	}

	if (!m_CameraZoomInputAction)
	{
		static ConstructorHelpers::FObjectFinder<UInputAction> cameraZoomInputActionFinder(TEXT("/Game/Input/IA_Camera_Zoom.IA_Camera_Zoom"));
		if (cameraZoomInputActionFinder.Succeeded())
		{
			m_CameraZoomInputAction = cameraZoomInputActionFinder.Object;
		}
	}

#pragma endregion
}

void ATCPawn::BeginPlay()
{
	Super::BeginPlay();
	m_TargetZoomLength = m_SpringArmComp->TargetArmLength;
	m_TargetLocation = GetActorLocation();
	m_TargetRotation = GetActorRotation();

	// 기본 입력 매핑 컨텍스트 설정 
	if (APlayerController* pPC = Cast<APlayerController>(GetController()))
	{
		if (ULocalPlayer* pLocalPlayer = pPC->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = pLocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				Subsystem->AddMappingContext(m_DefaultInputMappingContext, 0);
			}
		}
	}
}

void ATCPawn::Tick(float _dt)
{
	Super::Tick(_dt);
	_TickZoom(_dt);
	_TickMoving(_dt);
	_TickRotation(_dt);
}


void ATCPawn::SetupPlayerInputComponent(UInputComponent* _pInputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);
	if (UEnhancedInputComponent* pEnhancedInput = Cast<UEnhancedInputComponent>(_pInputComponent))
	{
		pEnhancedInput->BindAction(m_CameraMovingInputAction, ETriggerEvent::Triggered, this, &ATCPawn::_HandleMoving);
		pEnhancedInput->BindAction(m_CameraRotationInputAction, ETriggerEvent::Triggered, this, &ATCPawn::_HandleRotation);
		pEnhancedInput->BindAction(m_CameraZoomInputAction, ETriggerEvent::Triggered, this, &ATCPawn::_HandleZoom);
	}	
}

void ATCPawn::Zoom(float _value)
{
	static constexpr float zoomSensitivity = 40.0f;
	static constexpr float minZoom = 100.0f;
	static constexpr float maxZoom = 5000.0f;
	
	m_TargetZoomLength = FMath::Clamp(m_TargetZoomLength + (zoomSensitivity *_value), minZoom, maxZoom);	
}

void ATCPawn::MoveToForward(float _value)
{
	static constexpr float movingSensitivity = 20.0f;
	m_TargetLocation = m_TargetLocation + (_value * movingSensitivity * GetActorForwardVector());
}

void ATCPawn::MoveToRight(float _value)
{
	static constexpr float movingSensitivity = 20.0f;
	m_TargetLocation = m_TargetLocation + (_value * movingSensitivity * GetActorRightVector());
}

void ATCPawn::RotateRight()
{
	static constexpr float rotationSensitivity = 45.0f;
	m_TargetRotation -= FRotator(0.0f, rotationSensitivity, 0.0f);
}

void ATCPawn::RotateLeft()
{
	static constexpr float rotationSensitivity = 45.0f;
	m_TargetRotation += FRotator(0.0f, rotationSensitivity, 0.0f);
}

void ATCPawn::_HandleZoom(const FInputActionInstance& _instance)
{
	float value = _instance.GetValue().Get<float>();
	Zoom(value);
}

void ATCPawn::_HandleMoving(const FInputActionInstance& _instance)
{
	FVector2D value =  _instance.GetValue().Get<FVector2D>();
	value.Normalize();

	MoveToForward(value.X);
	MoveToRight(value.Y);
}

void ATCPawn::_HandleRotation(const FInputActionInstance& _instance)
{
	float value = _instance.GetValue().Get<float>();
	if (value < 0.0f)
	{
		RotateRight();
		return;
	}
	
	if (value > 0.0f)
	{
		RotateLeft();
		return;
	}
}

void ATCPawn::_TickZoom(float _dt)
{
	static constexpr float interpSpeed = 2.0f; 
	float adjustedLength  = FMath::FInterpTo(m_SpringArmComp->TargetArmLength, m_TargetZoomLength, _dt, interpSpeed);
	m_SpringArmComp->TargetArmLength = adjustedLength;;
}

void ATCPawn::_TickMoving(float _dt)
{
	static constexpr float interpSpeed = 5.0f; 
	FVector adjustedLocation = FMath::VInterpTo(GetActorLocation(), m_TargetLocation, _dt, interpSpeed);
	SetActorLocation(adjustedLocation);
}

void ATCPawn::_TickRotation(float _dt)
{
	static constexpr float interpSpeed = 50.0f;
	FRotator adjustedRotation = FMath::RInterpTo(GetActorRotation(), m_TargetRotation, _dt, interpSpeed);
	SetActorRotation(adjustedRotation);
}
