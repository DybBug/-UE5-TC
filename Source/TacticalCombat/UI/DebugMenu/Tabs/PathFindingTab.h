// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PathFindingTab.generated.h"

class UCheckBox;
class UDebugTextOnTiles;
/**
 * 
 */
UCLASS()
class TACTICALCOMBAT_API UPathFindingTab : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

#pragma region  Widgets
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UCheckBox> CheckBox_IsShowIndicesOnTiles;
#pragma endregion
	
#pragma region Internals
	UPROPERTY(VisibleInstanceOnly, Category = "Internal", Meta = (DisplayName = "Debug Text On Tiles"))
	TObjectPtr<UDebugTextOnTiles> m_DebugTextOnTiles;
#pragma endregion

protected:
	UFUNCTION()
	void OnCheckStateChanged(bool _bIsChecked);
};
