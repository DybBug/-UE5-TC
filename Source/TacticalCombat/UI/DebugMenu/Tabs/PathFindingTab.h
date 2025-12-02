// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PathFindingTab.generated.h"

class UCheckBox;
class UDebugTextAndColorOnTiles;
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

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UCheckBox> CheckBox_IsShowCostToEnterTile;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UCheckBox> CheckBox_IsShowMinCostToTarget;
	
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UCheckBox> CheckBox_IsShowCostFromStart;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UCheckBox> CheckBox_IsShowSortOrder;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UCheckBox> CheckBox_IsShowDiscoveredTiles;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UCheckBox> CheckBox_IsShowAnalyzedTiles;
#pragma endregion
	
#pragma region Internals
	UPROPERTY(VisibleInstanceOnly, Category = "Internal", Meta = (DisplayName = "Debug Text On Tiles"))
	TObjectPtr<UDebugTextAndColorOnTiles> m_DebugTextOnTiles;
#pragma endregion

protected:
	UFUNCTION()
	void OnIsShowIndicesOnTilesCheckStateChanged(bool _bIsChecked);

	UFUNCTION()
	void OnIsShowCostToEnterTileCheckStateChanged(bool _bIsChecked);

	UFUNCTION()
	void OnIsShowMinCostToTargetCheckStateChanged(bool _bIsChecked);

	UFUNCTION()
	void OnIsShowCostFromStartCheckStateChanged(bool _bIsChecked);

	UFUNCTION()
	void OnIsShowSortOrderCheckStateChanged(bool _bIsChecked);

	UFUNCTION()
	void OnIsShowDiscoveredTilesCheckStateChanged(bool _bIsChecked);

	UFUNCTION()
	void OnIsShowAnalyzedTilesCheckStateChanged(bool _bIsChecked);
};
