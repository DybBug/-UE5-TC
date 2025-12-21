// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UnitButtonList.generated.h"

class UHorizontalBox;
class UUnitButton;
enum class ETacticalUnitType: uint8;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSelectedUnitButtonChanged, ETacticalUnitType, _unitType);

/**
 * 
 */
UCLASS()
class TACTICALCOMBAT_API UUnitButtonList : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativePreConstruct() override;

public:
	UPROPERTY()
	FOnSelectedUnitButtonChanged OnSelectedUnitButtonChanged;

	ETacticalUnitType GetSelectedUnitType() const;
	
public:
#pragma region Properties
	UPROPERTY(EditAnywhere, Category = "Property", Meta = (DisplayName = "Button Class"))
	TSubclassOf<UUnitButton> m_ButtonClass;

	UPROPERTY(EditAnywhere, Category = "Property", Meta = (DisplayName = "Is Looking Right"))
	bool m_bIsLookingRight;

	UPROPERTY(EditDefaultsOnly, Category = "Property", Meta = (DisplayName = "Selected Unit Type On Construct"))
	ETacticalUnitType m_SelectedUnitTypeOnConstruct;
#pragma endregion

#pragma region Widgets
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UHorizontalBox> HorizontalBox;
#pragma region

#pragma region Internals
	uint8 m_SelectedButtonIndex;
#pragma endregion

private:
	UFUNCTION()
	void _OnButtonClicked(uint8 _index);
};
