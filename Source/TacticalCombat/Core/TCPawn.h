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

	FORCEINLINE void SetMovingSensitivity(float _value) { m_MovingSensitivity = _value; }
	FORCEINLINE void SetMovingInterpSpeed(float _value) { m_MovingInterpSpeed = _value; }
	FORCEINLINE void SetRotationSensitivity(float _value) { m_RotationSensitivity = _value; }
	FORCEINLINE void SetRotationInterpSpeed(float _value) { m_RotationInterpSpeed = _value; }
	FORCEINLINE void SetZoomSensitivity(float _value) { m_ZoomSensitivity = _value; }
	FORCEINLINE void SetZoomInterpSpeed(float _value) { m_ZoomInterpSpeed = _value; }
	FORCEINLINE void SetMinZoom(float _value) { m_MinZoom = _value; }
	FORCEINLINE void SetMaxZoom(float _value) { m_MaxZoom = _value; }

	FORCEINLINE float GetMovingSensitivity() const { return m_MovingSensitivity; }
	FORCEINLINE float GetMovingInterpSpeed() const { return m_MovingInterpSpeed; }
	FORCEINLINE float GetRotationSensitivity() const { return m_RotationSensitivity; }
	FORCEINLINE float GetRotationInterpSpeed() const { return m_RotationInterpSpeed; }
	FORCEINLINE float GetZoomSensitivity() const { return m_ZoomSensitivity; }
	FORCEINLINE float GetZoomInterpSpeed() const { return m_ZoomInterpSpeed; }
	FORCEINLINE float GetMinZoom() const { return m_MinZoom; }
	FORCEINLINE float GetMaxZoom() const { return m_MaxZoom; }
	
protected:
#pragma region Components
	UPROPERTY(EditAnywhere, Category = "Component", Meta = (DisplayName = "Root"))
	TObjectPtr<USceneComponent> m_SceneComp;

	UPROPERTY(EditAnywhere, Category = "Component", Meta = (DisplayName = "Spring Arm"))
	TObjectPtr<USpringArmComponent> m_SpringArmComp;
	
	UPROPERTY(EditAnywhere, Category = "Component", Meta = (DisplayName = "Camera"))
	TObjectPtr<UCameraComponent> m_CameraComp;
#pragma endregion

#pragma region Properties
	UPROPERTY(EditAnywhere, Category = "Property", Meta = (DisplayName = "Moving Speed"))
	float m_MovingSensitivity = 20.0f;

	UPROPERTY(EditAnywhere, Category = "Property", Meta = (DisplayName = "Moving Interp Speed"))
	float m_MovingInterpSpeed = 5.0f;
	
	UPROPERTY(EditAnywhere, Category = "Property", Meta = (DisplayName = "Rotation Sensitivity"))
	float m_RotationSensitivity = 45.0f;

	UPROPERTY(EditAnywhere, Category = "Property", Meta = (DisplayName = "Rotation Interp Speed"))
	float m_RotationInterpSpeed = 50.0f;

	UPROPERTY(EditAnywhere, Category = "Property", Meta = (DisplayName = "Zoom Sensitivity"))
	float m_ZoomSensitivity = 40.0f;

	UPROPERTY(EditAnywhere, Category = "Property", Meta = (DisplayName = "Zoom Interp Speed"))
	float m_ZoomInterpSpeed = 2.0f;
	
	UPROPERTY(EditAnywhere, Category = "Property", Meta = (DisplayName = "Min Zoom"))
	float m_MinZoom = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Property", Meta = (DisplayName = "Max Zoom"))
	float m_MaxZoom = 5000.0f;

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

