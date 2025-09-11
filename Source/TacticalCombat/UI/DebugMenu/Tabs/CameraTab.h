// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CameraTab.generated.h"

class ATCPawn;
class UWithNameSpinBox;
/**
 * 
 */
UCLASS()
class TACTICALCOMBAT_API UCameraTab : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UWithNameSpinBox> SpinBox_MovingSensitivity;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UWithNameSpinBox> SpinBox_MovingInterpSpeed;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UWithNameSpinBox> SpinBox_RotationSensitivity;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UWithNameSpinBox> SpinBox_RotationInterpSpeed;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UWithNameSpinBox> SpinBox_ZoomSensitivity;
	
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UWithNameSpinBox> SpinBox_ZoomInterpSpeed;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UWithNameSpinBox> SpinBox_MinZoom;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UWithNameSpinBox> SpinBox_MaxZoom;

	UPROPERTY()
	TWeakObjectPtr<ATCPawn> m_Pawn;

private:
	UFUNCTION()
	void _OnMovingSensitivityChanged(float _value);

	UFUNCTION()
	void _OnMovingInterpSeedChanged(float _value);
	
	UFUNCTION()
	void _OnRotationSensitivityChanged(float _value);

	UFUNCTION()
	void _OnRotationInterpSeedChanged(float _value);

	UFUNCTION()
	void _OnZoomSensitivityChanged(float _value);

	UFUNCTION()
	void _OnZoomInterpSpeedChanged(float _value);

	UFUNCTION()
	void _OnMinZoomChanged(float _value);

	UFUNCTION()
	void _OnMaxZoomChanged(float _value);
};
