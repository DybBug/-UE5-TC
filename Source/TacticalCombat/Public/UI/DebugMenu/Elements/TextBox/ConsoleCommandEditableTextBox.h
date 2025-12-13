// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/EditableTextBox.h"
#include "ConsoleCommandEditableTextBox.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class TACTICALCOMBAT_API UConsoleCommandEditableTextBox : public UEditableTextBox
{
	GENERATED_BODY()

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

private:
	UFUNCTION()
	void _OnTextCommitted(const FText& _text, ETextCommit::Type _commitMethod);	
};
