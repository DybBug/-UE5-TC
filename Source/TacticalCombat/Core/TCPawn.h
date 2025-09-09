// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TCPawn.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionInstance;

UCLASS()
class TACTICALCOMBAT_API ATCPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATCPawn();

protected:	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;	

public:
	virtual void Tick(float _dt) override;
	virtual void SetupPlayerInputComponent(UInputComponent* _pInputComponent) override;

protected:
#pragma region Components
	UPROPERTY(EditAnywhere, Category = "Component", Meta = (DisplayName = "Root"))
	TObjectPtr<USceneComponent> m_SceneComp;

	UPROPERTY(EditAnywhere, Category = "Component", Meta = (DisplayName = "Spring Arm"))
	TObjectPtr<USpringArmComponent> m_SpringArmComp;
	
	UPROPERTY(EditAnywhere, Category = "Component", Meta = (DisplayName = "Camera"))
	TObjectPtr<UCameraComponent> m_CameraComp;
#pragma endregion

#pragma region Inputs
	UPROPERTY(EditAnywhere, Category = "Input", Meta = (DisplayName = "Default Input Mapping Context"))
	TObjectPtr<UInputMappingContext> m_DefaultInputMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input", Meta = (DisplayName = "Camera Moving Input Action"))
	TObjectPtr<UInputAction> m_CameraMovingInputAction;

	UPROPERTY(EditAnywhere, Category = "Input", Meta = (DisplayName = "Camera Rotation Input Action"))
	TObjectPtr<UInputAction> m_CameraRotationInputAction;

	UPROPERTY(EditAnywhere, Category = "Input", Meta = (DisplayName = "Camera Zoom Input Action"))
	TObjectPtr<UInputAction> m_CameraZoomInputAction;
#pragma endregion
private:
	float m_TargetZoomLength = 0.0f;
	FVector m_TargetLocation = FVector::ZeroVector;
	FRotator m_TargetRotation = FRotator::ZeroRotator;

public:
	void Zoom(float _value);
	void MoveToForward(float _value);
	void MoveToRight(float _value);
	void RotateRight();
	void RotateLeft();

private:
	UFUNCTION()
	void _HandleZoom(const FInputActionInstance& _instance);

	UFUNCTION()
	void _HandleMoving(const FInputActionInstance& _instance);
	
	UFUNCTION()
	void _HandleRotation(const FInputActionInstance& _instance);
	
	void _TickZoom(float _dt);
	void _TickMoving(float _dt);
	void _TickRotation(float _dt);
};

