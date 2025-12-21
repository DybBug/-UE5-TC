// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Shared/SharedEnums.h"
#include "UnitButton.generated.h"

class UBorder;
class UButton;
class UImage;
enum class ETacticalUnitType: uint8;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnButtonClicked, uint8, index);
/**
 * 
 */
UCLASS()
class TACTICALCOMBAT_API UUnitButton : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativePreConstruct() override;

public:
	struct FInitializeParams
	{
		uint8 Index;
		ETacticalUnitType UnitType;
		bool bIsLookingRight;
		bool bIsSelected;

		FInitializeParams() = delete;
		FInitializeParams(uint8 _index, ETacticalUnitType _type,  bool _bIsLookingRight, bool _bIsSelected)
			:Index(_index), UnitType(_type),  bIsLookingRight(_bIsLookingRight), bIsSelected(_bIsSelected) { }
		
	};
	void InitializeButton(const FInitializeParams& params);

#pragma region Getter
	FORCEINLINE bool IsSelected() const { return m_bIsSelected;}
	FORCEINLINE ETacticalUnitType GetUnitType() const {	return m_UnitType; }
#pragma endregion
	
#pragma region Setter
	FORCEINLINE void SetIndex(uint8 _index) { m_Index = _index;}
	FORCEINLINE void SetUnitType(ETacticalUnitType _unitType) { m_UnitType = _unitType; }
	FORCEINLINE void SetIsLookingRight(bool _isLookingRight) { m_bIsLookingRight = _isLookingRight; }
	FORCEINLINE void SetIsSelected(bool _isSelected);
#pragma endregion
public :
	UPROPERTY()
	FOnButtonClicked OnButtonClicked;
	
protected:
#pragma region Properties
	UPROPERTY(EditAnywhere, Category = "Property", Meta = (DisplayName = "Unit Type"))
	ETacticalUnitType m_UnitType;

	UPROPERTY(EditAnywhere, Category = "Property", Meta = (DisplayName = "Is Looking Right"))
	bool m_bIsLookingRight;

	UPROPERTY(EditAnywhere, Category = "Property", Meta = (DisplayName = "Is Selected"))
	bool m_bIsSelected;
#pragma endregion
	
#pragma region Widgets
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UBorder> Border;
	
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> Button;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UImage> Image;
#pragma endregion

#pragma region Internals
	uint8 m_Index;
#pragma endregion

private:
	UFUNCTION()
	void _OnButtonClicked();

};
