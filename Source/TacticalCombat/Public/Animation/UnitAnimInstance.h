// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Interface/UnitAnimationInterface.h"
#include "UnitAnimInstance.generated.h"

enum class EUnitAnimationState: uint8;
/**
 * 
 */
UCLASS()
class TACTICALCOMBAT_API UUnitAnimInstance : public UAnimInstance, public IUnitAnimationInterface
{
	GENERATED_BODY()

public:
	UUnitAnimInstance() = default;
	
public :
	virtual void SetUnitAnimationState_Implementation(EUnitAnimationState _newState);


protected:
#pragma region Properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Property", Meta = (DisplayName = "Animation State"))
	EUnitAnimationState m_UnitAnimationState;
#pragma endregion

#pragma region Internals
	FTimerHandle m_hStateUpdateTimer;
	FTimerHandle m_hStateBackToIdleTimer;
#pragma endregion

private:
	void _ApplyUnitAnimationState(EUnitAnimationState _newState);
	void _ResetToIdleState();
	
};
