// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ActionButton.generated.h"

class UAbstractAction;
class UButton;
class UTextBlock;
class APlayerActions;
/**
 * 
 */
UCLASS()
class TACTICALCOMBAT_API UActionButton : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

protected:
	virtual void NativePreConstruct() override;

protected:
#pragma region Widgets
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> Button;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTextBlock> Text;
#pragma endregion

#pragma region Properties
	UPROPERTY(EditAnywhere, Category = "Property", Meta = (DisplayName = "Action Text"))
	FString m_ActionText;

	UPROPERTY(EditAnywhere, Category = "Property", Meta = (DisplayName = "Left Click Action Class"))
	TSubclassOf<UAbstractAction> m_LeftClickActionClass;

	UPROPERTY(EditAnywhere, Category = "Property", Meta = (DisplayName = "Right Click Action Class"))
	TSubclassOf<UAbstractAction> m_RightClickActionClass;
#pragma endregion

#pragma region Internal
	UPROPERTY(VisibleAnywhere, Category = "Internal", Meta = (DisplayName = "Player Actions"))
	TWeakObjectPtr<APlayerActions> m_PlayerActions;
#pragma endregion

protected:
	UFUNCTION()
	virtual void OnSelectedActionsChanged(const UAbstractAction* const _leftClickAction, const UAbstractAction* const _rightClickAction);

private:
	UFUNCTION()
	void _OnButtonClicked();
};
