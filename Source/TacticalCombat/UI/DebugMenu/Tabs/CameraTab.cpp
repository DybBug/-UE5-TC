// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraTab.h"

#include "TacticalCombat/Core/TCPawn.h"
#include "TacticalCombat/UI/DebugMenu/Elements/SpinBox/WithNameSpinBox.h"

void UCameraTab::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	UE_LOG(LogTemp, Warning, TEXT("NativeOnInitialized : %s"), SpinBox_MovingSensitivity == nullptr ? TEXT("invalid") : TEXT("valid"));
}

void UCameraTab::NativePreConstruct()
{
	Super::NativePreConstruct();
	UE_LOG(LogTemp, Warning, TEXT("NativePreConstruct : %s"), SpinBox_MovingSensitivity == nullptr ? TEXT("invalid") : TEXT("valid"));
}

void UCameraTab::NativeConstruct()
{
	Super::NativeConstruct();
	UE_LOG(LogTemp, Warning, TEXT("NativeConstruct : %s"), SpinBox_MovingSensitivity == nullptr ? TEXT("invalid") : TEXT("valid"));
	
	m_Pawn = Cast<ATCPawn>(GetOwningPlayerPawn());
	if (m_Pawn.IsValid())
	{
		SpinBox_MovingSensitivity->SetValue(m_Pawn->GetMovingSensitivity());
		SpinBox_MovingInterpSpeed->SetValue(m_Pawn->GetMovingInterpSpeed());
		SpinBox_RotationSensitivity->SetValue(m_Pawn->GetRotationSensitivity());
		SpinBox_RotationInterpSpeed->SetValue(m_Pawn->GetRotationInterpSpeed());
		SpinBox_ZoomSensitivity->SetValue(m_Pawn->GetZoomSensitivity());
		SpinBox_ZoomInterpSpeed->SetValue(m_Pawn->GetZoomInterpSpeed());
		SpinBox_MinZoom->SetValue(m_Pawn->GetMinZoom());
		SpinBox_MaxZoom->SetValue(m_Pawn->GetMaxZoom());
	}	

	SpinBox_MovingSensitivity->OnValueChanged.AddDynamic(this, &UCameraTab::_OnMovingSensitivityChanged);
	SpinBox_MovingInterpSpeed->OnValueChanged.AddDynamic(this, &UCameraTab::_OnMovingInterpSeedChanged);
	SpinBox_RotationSensitivity->OnValueChanged.AddDynamic(this, &UCameraTab::_OnRotationSensitivityChanged);
	SpinBox_RotationInterpSpeed->OnValueChanged.AddDynamic(this, &UCameraTab::_OnRotationInterpSeedChanged);
	SpinBox_ZoomSensitivity->OnValueChanged.AddDynamic(this, &UCameraTab::_OnZoomSensitivityChanged);
	SpinBox_ZoomInterpSpeed->OnValueChanged.AddDynamic(this, &UCameraTab::_OnZoomInterpSpeedChanged);
	SpinBox_MinZoom->OnValueChanged.AddDynamic(this, &UCameraTab::_OnMinZoomChanged);
	SpinBox_MaxZoom->OnValueChanged.AddDynamic(this, &UCameraTab::_OnMaxZoomChanged);
	
}

void UCameraTab::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	UE_LOG(LogTemp, Warning, TEXT("SynchronizeProperties : %s"), SpinBox_MovingSensitivity == nullptr ? TEXT("invalid") : TEXT("valid"));
}

void UCameraTab::_OnMovingSensitivityChanged(float _value)
{
	if (!m_Pawn.IsValid()) return;
	m_Pawn->SetMovingSensitivity(_value);
}

void UCameraTab::_OnMovingInterpSeedChanged(float _value)
{
	if (!m_Pawn.IsValid()) return;
	m_Pawn->SetMovingInterpSpeed(_value);
}

void UCameraTab::_OnRotationSensitivityChanged(float _value)
{
	if (!m_Pawn.IsValid()) return;
	m_Pawn->SetRotationSensitivity(_value);
}

void UCameraTab::_OnRotationInterpSeedChanged(float _value)
{
	if (!m_Pawn.IsValid()) return;
	m_Pawn->SetRotationInterpSpeed(_value);
}

void UCameraTab::_OnZoomSensitivityChanged(float _value)
{
	if (!m_Pawn.IsValid()) return;
	m_Pawn->SetZoomSensitivity(_value);
}

void UCameraTab::_OnZoomInterpSpeedChanged(float _value)
{
	if (!m_Pawn.IsValid()) return;
	m_Pawn->SetZoomInterpSpeed(_value);
}

void UCameraTab::_OnMinZoomChanged(float _value)
{
	if (!m_Pawn.IsValid()) return;
	m_Pawn->SetMinZoom(_value);
}

void UCameraTab::_OnMaxZoomChanged(float _value)
{
	if (!m_Pawn.IsValid()) return;
	m_Pawn->SetMaxZoom(_value);
}