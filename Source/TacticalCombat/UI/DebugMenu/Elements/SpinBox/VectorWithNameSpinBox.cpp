// Fill out your copyright notice in the Description page of Project Settings.


#include "VectorWithNameSpinBox.h"

#include "Components/SpinBox.h"
#include "Components/TextBlock.h"


void UVectorWithNameSpinBox::NativeConstruct()
{
	Super::NativeConstruct();

	SpinBoxX->OnValueChanged.AddDynamic(this, &UVectorWithNameSpinBox::_OnValueXChanged);
	SpinBoxY->OnValueChanged.AddDynamic(this, &UVectorWithNameSpinBox::_OnValueYChanged);
	SpinBoxZ->OnValueChanged.AddDynamic(this, &UVectorWithNameSpinBox::_OnValueZChanged);
}

void UVectorWithNameSpinBox::NativePreConstruct()
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

#pragma region Setup SpinBox Z
	SpinBoxZ->SetMinSliderValue(m_SliderRange.X);
	SpinBoxZ->SetMaxSliderValue(m_SliderRange.Y);
	
	SpinBoxZ->SetMinValue(m_ValueRange.X);
	SpinBoxZ->SetMaxValue(m_ValueRange.Y);

	SpinBoxZ->SetValue(m_Value.Z);
	
	SpinBoxZ->SetMinFractionalDigits(FMath::TruncToFloat(m_FractionalDigitsRange.X));
	SpinBoxZ->SetMaxFractionalDigits(FMath::TruncToFloat(m_FractionalDigitsRange.Y));

	SpinBoxZ->SetAlwaysUsesDeltaSnap(m_bIsAlwaysSnapToDelta);
	SpinBoxZ->SetDelta(m_Delta);
#pragma endregion
}

void UVectorWithNameSpinBox::SetValue(const FVector& _value)
{
	m_Value = _value;
	SpinBoxX->SetValue(m_Value.X);
	SpinBoxY->SetValue(m_Value.Y);
	SpinBoxZ->SetValue(m_Value.Z);
}

void UVectorWithNameSpinBox::_OnValueXChanged(float _x)
{
	FVector value = FVector(_x, SpinBoxY->GetValue(), SpinBoxZ->GetValue());
	m_Value = value;
	if (OnValueChanged.IsBound())
	{
		OnValueChanged.Broadcast(m_Value);
	}
}

void UVectorWithNameSpinBox::_OnValueYChanged(float _y)
{
	FVector value = FVector(SpinBoxX->GetValue(), _y, SpinBoxZ->GetValue());
	m_Value = value;
	if (OnValueChanged.IsBound())
	{
		OnValueChanged.Broadcast(m_Value);
	}
}

void UVectorWithNameSpinBox::_OnValueZChanged(float _z)
{
	FVector value = FVector(SpinBoxX->GetValue(), SpinBoxY->GetValue(), _z);
	m_Value = value;
	if (OnValueChanged.IsBound())
	{
		OnValueChanged.Broadcast(m_Value);
	}
}