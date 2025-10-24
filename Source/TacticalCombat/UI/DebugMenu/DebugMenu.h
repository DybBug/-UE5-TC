// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DebugMenu.generated.h"

class UHorizontalBox;
class UWidgetSwitcher;
class UButton;
/**
 * 
 */
UCLASS()
class TACTICALCOMBAT_API UDebugMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;


protected:
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UHorizontalBox> HorizontalBox;
	
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> Button_Tab1;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> Button_Tab2;
	
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> Button_Tab3;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> Button_Tab4;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> WidgetSwitcher;

private:
	UFUNCTION()
	void _OnClicked_Tab1();

	UFUNCTION()
	void _OnClicked_Tab2();

	UFUNCTION()
	void _OnClicked_Tab3();

	UFUNCTION()
	void _OnClicked_Tab4();

private:
	void _ToggleTabIndex(int32 _tabIndex);
	void _UpdateAllButtonTabs();
};
