// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Interface/UnitAnimationInterface.h"

// Add default functionality here for any IUnitAnimationInterface functions that are not pure virtual.
void IUnitAnimationInterface::SetUnitAnimationState_Implementation(EUnitAnimationState _newState)
{
	UE_LOG(LogTemp, Warning, TEXT("SetUnitAnimationState not implemented in child class"));
}
