// Fill out your copyright notice in the Description page of Project Settings.


#include "Library/UtilityLibrary.h"

FVector UUtilityLibrary::SnapVectorToVector(const FVector& _lhs, const FVector& _rhs)
{
	float x = FMath::GridSnap(_lhs.X, _rhs.X);
	float y = FMath::GridSnap(_lhs.Y, _rhs.Y);
	float z = FMath::GridSnap(_lhs.Z, _rhs.Z);
	return FVector(x, y, z);
}

bool UUtilityLibrary::IsFloatEven(float _value)
{	
	return  FMath::Abs(FMath::Fmod(_value, 2.0f)) <= FLT_EPSILON;
}

bool UUtilityLibrary::IsIntEven(int32 _value)
{
	return _value % 2 == 0;
}