// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/DebugMenu/Elements/Button/Action/ActionButton.h"
#include "CastSpellActionButton.generated.h"

class UComboBoxString;
/**
 * 
 */
UCLASS()
class TACTICALCOMBAT_API UCastSpellActionButton : public UActionButton
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
public:
	virtual void NativePreConstruct() override;

protected:
#pragma region Widgets
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UComboBoxString> ComboBoxString;
#pragma endregion

	FTimerHandle m_hDelayTimer;

protected:
	virtual void OnSelectedActionsChanged(const UAbstractAction* const _leftClickAction, const UAbstractAction* const _rightClickAction) override;

private:
	UFUNCTION()
	void _OnComboBoxSpellTypeSelectionChanged(FString _selectedItem, ESelectInfo::Type _selectionType);

	void _ApplySelectedSpellType();
};
