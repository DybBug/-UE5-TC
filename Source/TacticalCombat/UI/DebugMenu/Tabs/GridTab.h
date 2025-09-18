// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GridTab.generated.h"

class UWithNameSpinBox;
class UVector2DWithNameSpinBox;
class UVectorWithNameSpinBox;
class UComboBoxString;
class AGrid;
/**
 * 
 */
UCLASS()
class TACTICALCOMBAT_API UGridTab : public UUserWidget
{
	GENERATED_BODY()	

protected:
	virtual void NativeConstruct() override;
	
protected:
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UComboBoxString> ComboBox_GridShape;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UWithNameSpinBox> SpinBox_ReGenDelay;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UVectorWithNameSpinBox> SpinBox_Location;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UVector2DWithNameSpinBox> SpinBox_TileCount;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UVectorWithNameSpinBox> SpinBox_TileSize;
	
	TWeakObjectPtr<AGrid> m_GridInWorld;

	FTimerHandle m_hSpawnTimer;

private:
	bool _TrySetDefaultValues();
	bool _TrySpawnGrid();
	
	UFUNCTION()
	void _OnGridShapeSelectionChanged(FString _selectedItem, ESelectInfo::Type _selectionType);
	
	UFUNCTION()
	void _OnLocationChanged(const FVector& _value);

	UFUNCTION()
	void _OnTileCountChanged(const FVector2D& _value);

	UFUNCTION()
	void _OnTileSizeChanged(const FVector& _value);

	UFUNCTION()
	void _ExecuteSpawnTimer();
};