// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DebugMenu/Elements/UnitButtonList.h"

#include "Components/HorizontalBox.h"
#include "Shared/SharedEnums.h"
#include "UI/DebugMenu/Elements/Button/UnitButton.h"

void UUnitButtonList::NativePreConstruct()
{
	Super::NativePreConstruct();

	HorizontalBox->ClearChildren();
	if (m_ButtonClass)
	{
		uint8 currButtonIndex = 0;
		for (const ETacticalUnitType& unitType :TEnumRange<ETacticalUnitType>())
		{			
			UUnitButton* pUnitButton = CreateWidget<UUnitButton>(this, m_ButtonClass);
			pUnitButton->InitializeButton(UUnitButton::FInitializeParams(
				currButtonIndex,
				unitType,
				m_bIsLookingRight,
				(m_SelectedUnitTypeOnConstruct == unitType)
			));
			pUnitButton->OnButtonClicked.AddDynamic(this, &UUnitButtonList::_OnButtonClicked);
			HorizontalBox->AddChild(pUnitButton);

			if (pUnitButton->IsSelected())
			{
				m_SelectedButtonIndex = currButtonIndex;
			}
			++currButtonIndex;
		}
	}
}

ETacticalUnitType UUnitButtonList::GetSelectedUnitType() const
{
	UUnitButton* pUnitButton = Cast<UUnitButton>(HorizontalBox->GetChildAt(m_SelectedButtonIndex));
	return pUnitButton->GetUnitType();
}

void UUnitButtonList::_OnButtonClicked(uint8 _index)
{
	if (m_SelectedButtonIndex != _index)
	{
		UUnitButton* pPrevSelectedUnitButton = Cast<UUnitButton>(HorizontalBox->GetChildAt(m_SelectedButtonIndex));
		pPrevSelectedUnitButton->SetIsSelected(false);

		UUnitButton* pCurrentSelectedUnitButton = Cast<UUnitButton>(HorizontalBox->GetChildAt(_index));
		pCurrentSelectedUnitButton->SetIsSelected(true);

		m_SelectedButtonIndex = _index;

		if (OnSelectedUnitButtonChanged.IsBound())
		{
			OnSelectedUnitButtonChanged.Broadcast(pCurrentSelectedUnitButton->GetUnitType());
		}
	}
}
