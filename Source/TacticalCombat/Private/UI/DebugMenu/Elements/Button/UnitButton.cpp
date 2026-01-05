// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DebugMenu/Elements/Button/UnitButton.h"

#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Library/UnitsLibrary.h"

void UUnitButton::NativePreConstruct()
{
	Super::NativeConstruct();

	FUnitTableRow tableRow = UUnitsLibrary::GetDefaultUnitAssetsDataFromUnitType(m_UnitType);
	Image->SetBrushFromTexture(tableRow.Assets.Icon);
	FVector2D renderScale = FVector2D( m_bIsLookingRight ? 1.0f : -1.0f, 1.0f);
	Image->SetRenderScale(renderScale);

	SetIsSelected(m_bIsSelected);

	Button->OnClicked.AddDynamic(this, &UUnitButton::_OnButtonClicked);
}

void UUnitButton::InitializeButton(const FInitializeParams& params)
{
	SetIndex(params.Index);
	SetUnitType(params.UnitType);
	SetIsLookingRight(params.bIsLookingRight);
	SetIsSelected(params.bIsSelected);
}

void UUnitButton::SetIsSelected(bool _isSelected)
{
	m_bIsSelected = _isSelected;
	Border->SetBrushColor(m_bIsSelected ? FColor(255, 255, 255, 255) : FColor(0, 0, 0, 0));
	
}

void UUnitButton::_OnButtonClicked()
{
	SetIsSelected(!m_bIsSelected);
	if (OnButtonClicked.IsBound())
	{
		OnButtonClicked.Broadcast(m_Index);
	}
}
