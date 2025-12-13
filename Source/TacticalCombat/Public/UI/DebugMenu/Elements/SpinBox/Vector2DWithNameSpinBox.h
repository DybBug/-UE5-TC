// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Vector2DWithNameSpinBox.generated.h"

class USpinBox;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVector2DChanged, const FVector2D&, _value);

/**
 * 
 */
UCLASS()
class TACTICALCOMBAT_API UVector2DWithNameSpinBox : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

protected:
	virtual void NativePreConstruct() override;

public:
	FORCEINLINE const FVector2D& GetValue() const { return m_Value; }
	
	void SetValue(const FVector2D& _value);	
	
	FOnVector2DChanged OnValueChanged;

protected:
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTextBlock> Text;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<USpinBox> SpinBoxX;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<USpinBox> SpinBoxY;
	
	UPROPERTY(EditAnywhere, Category = "Property", Meta = (DisplayName = "Name", ToolTip = "이 항목의 이름"))
	FText m_Name;

	UPROPERTY(EditAnywhere, Category = "Property", Meta = (DisplayName = "Slider Range", ToolTip = "슬라이더가 가질 수 있는 최소값과 최대값"))
	FVector2D m_SliderRange;

	UPROPERTY(EditAnywhere, Category = "Property", Meta = (DisplayName = "Value Range", ToolTip = "값이 허용되는 범위"))
	FVector2D m_ValueRange;

	UPROPERTY(EditAnywhere, Category = "Property", Meta = (DisplayName = "Value", ToolTip = "현재 설정된 값"))
	FVector2D m_Value;

	UPROPERTY(EditAnywhere, Category = "Property", Meta = (DisplayName = "Fractional Digits Range", ToolTip = "소수점 자릿수의 최소값과 최대값(표시 정밀도를 조절)"))
	FVector2D m_FractionalDigitsRange;

	UPROPERTY(EditAnywhere, Category = "Property", Meta = (DisplayName = "Is Always Snap To Delta", ToolTip = "이 옵션을 활성화하면 값이 항상 델타 값에 맞춰 스냅"))
	bool m_bIsAlwaysSnapToDelta;

	UPROPERTY(EditAnywhere, Category = "Property", Meta = (DisplayName = "Delta", ToolTip = "스냅 델타값"))
	float m_Delta;


private:
	UFUNCTION()
	void _OnValueXChanged(float _x);

	UFUNCTION()
	void _OnValueYChanged(float _y);
};
