// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DebugMenu/DebugMenu.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/WidgetSwitcher.h"

bool UDebugMenu::Initialize()
{
	bool result = Super::Initialize();

	_UpdateAllButtonTabs();
	
	Button_Tab1->OnClicked.AddDynamic(this, &UDebugMenu::_OnClicked_Tab1);
	Button_Tab2->OnClicked.AddDynamic(this, &UDebugMenu::_OnClicked_Tab2);
	Button_Tab3->OnClicked.AddDynamic(this, &UDebugMenu::_OnClicked_Tab3);	
	Button_Tab4->OnClicked.AddDynamic(this, &UDebugMenu::_OnClicked_Tab4);	
	return result;
}

void UDebugMenu::_OnClicked_Tab1()
{
	_ToggleTabIndex(1);
}

void UDebugMenu::_OnClicked_Tab2()
{
	_ToggleTabIndex(2);
}

void UDebugMenu::_OnClicked_Tab3()
{
	_ToggleTabIndex(3);
}

void UDebugMenu::_OnClicked_Tab4()
{
	_ToggleTabIndex(4);
}

void UDebugMenu::_ToggleTabIndex(int32 _tabIndex)
{
	int32 currentActiveWidgetIndex = WidgetSwitcher->GetActiveWidgetIndex();
	WidgetSwitcher->SetActiveWidgetIndex(currentActiveWidgetIndex == _tabIndex ? 0 : _tabIndex);
	
	_UpdateAllButtonTabs();
}

void UDebugMenu::_UpdateAllButtonTabs()
{
	constexpr FLinearColor activeColor = FLinearColor(0.000000f, 0.162029f, 0.745404f);
	constexpr FLinearColor inactiveColor = FLinearColor(0.028426f,0.028426f,0.028426f);
	
	const int32 currentActiveWidgetIndex = WidgetSwitcher->GetActiveWidgetIndex();
	
	int32 tabIndex = 0;
	for (int32 i = 0; i < HorizontalBox->GetChildrenCount(); ++i)
	{
		UButton* pButtonTab =  Cast<UButton>(HorizontalBox->GetChildAt(i));
		if (!pButtonTab) continue;

		++tabIndex;
		const bool isActive = (tabIndex == currentActiveWidgetIndex);
		pButtonTab->SetBackgroundColor(isActive ? activeColor : inactiveColor);
	}
}
