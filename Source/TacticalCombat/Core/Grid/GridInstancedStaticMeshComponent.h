// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "GridInstancedStaticMeshComponent.generated.h"

/**
 * 
 */
UCLASS()
class TACTICALCOMBAT_API UGridInstancedStaticMeshComponent : public UInstancedStaticMeshComponent
{
	GENERATED_BODY()

public:
	UGridInstancedStaticMeshComponent();
	
protected:
	using UInstancedStaticMeshComponent::AddInstance;
	using UInstancedStaticMeshComponent::RemoveInstance;
	
public:
	void AddInstance(const FIntPoint& _index, const FTransform& _transform, uint8 _tileStateMask);
	void RemoveInstance(const FIntPoint& _index);
	virtual void ClearInstances() override;
protected:
	UPROPERTY(VisibleAnywhere, Category = "Property", Meta = (DisplayName = "Instance Indices"))
	TArray<FIntPoint> m_InstanceIndices;

private:
	FColor _GetColorFromState(uint8 _tileStateMask);
	
};
