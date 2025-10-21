// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ConsoleCommandButton.generated.h"

class UButton;
class UTextBlock;
/**
 * 
 */
UCLASS()
class TACTICALCOMBAT_API UConsoleCommandButton : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

protected:
	virtual void NativePreConstruct() override;
	
protected:
	UPROPERTY(Meta=(BindWidget))
	TObjectPtr<UButton> Button;

	UPROPERTY(Meta=(BindWidget))
	TObjectPtr<UTextBlock> Text;

	UPROPERTY(EditAnywhere, Category = "Property", Meta = (DisplayName = "Console Command"))
	FString m_ConsoleCommand;

private:
	UFUNCTION()
	void _OnClickedButton();
		
	
};
