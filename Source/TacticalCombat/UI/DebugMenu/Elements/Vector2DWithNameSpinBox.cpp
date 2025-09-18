// Fill out your copyright notice in the Description page of Project Settings.


#include "Vector2DWithNameSpinBox.h"

#include "Components/SpinBox.h"
#include "Components/TextBlock.h"


void UVector2DWithNameSpinBox::NativeConstruct()
{
	Super::NativeConstruct();

	SpinBoxX->OnValueChanged.AddDynamic(this, &UVector2DWithNameSpinBox::_OnValueXChanged);
	SpinBoxY->OnValueChanged.AddDynamic(this, &UVector2DWithNameSpinBox::_OnValueYChanged);
}

void UVector2DWithNameSpinBox::NativePreConstruct()
{
	Super::NativePreConstruct();

	Text->SetText(m_Name);
	SetValue(m_Value);
	
#pragma region Setup SpinBox X 
	SpinBoxX->SetMinSliderValue(m_SliderRange.X);
	SpinBoxX->SetMaxSliderValue(m_SliderRange.Y);
	
	SpinBoxX->SetMinValue(m_ValueRange.X);
	SpinBoxX->SetMaxValue(m_ValueRange.Y);

	SpinBoxX->SetValue(m_Value.X);
	
	SpinBoxX->SetMinFractionalDigits(FMath::TruncToFloat(m_FractionalDigitsRange.X));
	SpinBoxX->SetMaxFractionalDigits(FMath::TruncToFloat(m_FractionalDigitsRange.Y));

	SpinBoxX->SetAlwaysUsesDeltaSnap(m_bIsAlwaysSnapToDelta);
	SpinBoxX->SetDelta(m_Delta);
#pragma endregion

#pragma region Setup SpinBox Y
	SpinBoxY->SetMinSliderValue(m_SliderRange.X);
	SpinBoxY->SetMaxSliderValue(m_SliderRange.Y);
	
	SpinBoxY->SetMinValue(m_ValueRange.X);
	SpinBoxY->SetMaxValue(m_ValueRange.Y);

	SpinBoxY->SetValue(m_Value.Y);
	
	SpinBoxY->SetMinFractionalDigits(FMath::TruncToFloat(m_FractionalDigitsRange.X));
	SpinBoxY->SetMaxFractionalDigits(FMath::TruncToFloat(m_FractionalDigitsRange.Y));

	SpinBoxY->SetAlwaysUsesDeltaSnap(m_bIsAlwaysSnapToDelta);
	SpinBoxY->SetDelta(m_Delta);
#pragma endregion
}

void UVector2DWithNameSpinBox::SetValue(const FVector2D& _value)
{
	m_Value = _value;
	SpinBoxX->SetValue(m_Value.X);
	SpinBoxY->SetValue(m_Value.Y);
}

void UVector2DWithNameSpinBox::_OnValueXChanged(float _x)
{
	FVector2D value = FVector2D(_x, SpinBoxY->GetValue());
	m_Value = value;
	if (OnValueChanged.IsBound())
	{
		OnValueChanged.Broadcast(m_Value);
	}
}

void UVector2DWithNameSpinBox::_OnValueYChanged(float _y)
{
	FVector2D value = FVector2D(SpinBoxX->GetValue(), _y);
	m_Value = value;
	if (OnValueChanged.IsBound())
	{
		OnValueChanged.Broadcast(m_Value);
	}
}