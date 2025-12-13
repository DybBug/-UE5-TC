// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DebugMenu/Elements/SpinBox/WithNameSpinBox.h"

#include "Components/SpinBox.h"
#include "Components/TextBlock.h"


void UWithNameSpinBox::NativeConstruct()
{
	Super::NativeConstruct();

	SpinBox->OnValueChanged.AddDynamic(this, &UWithNameSpinBox::_OnValueChanged);
}

void UWithNameSpinBox::NativePreConstruct()
{
	Super::NativePreConstruct();

	Text->SetText(m_Name);
	SetValue(m_Value);
	
	SpinBox->SetMinSliderValue(m_SliderRange.X);
	SpinBox->SetMaxSliderValue(m_SliderRange.Y);
	
	SpinBox->SetMinValue(m_ValueRange.X);
	SpinBox->SetMaxValue(m_ValueRange.Y);

	SpinBox->SetValue(m_Value);
	
	SpinBox->SetMinFractionalDigits(FMath::TruncToFloat(m_FractionalDigitsRange.X));
	SpinBox->SetMaxFractionalDigits(FMath::TruncToFloat(m_FractionalDigitsRange.Y));

	SpinBox->SetAlwaysUsesDeltaSnap(m_bIsAlwaysSnapToDelta);
	SpinBox->SetDelta(m_Delta);
}

void UWithNameSpinBox::SetValue(float _value)
{
	m_Value = _value;
	SpinBox->SetValue(m_Value);
}

void UWithNameSpinBox::_OnValueChanged(float _value)
{
	m_Value = _value;
	if (OnValueChanged.IsBound())
	{
		OnValueChanged.Broadcast(_value);
	}
}
