// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "GridInstancedStaticMeshComponent.generated.h"

enum class ETileType: uint8;
struct FTileStateInfo;

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
	void InitializeGridMeshInst(UStaticMesh* const _pMesh, UMaterialInstance* const _pMaterial, bool _IsColorBasedOnTileType, ECollisionEnabled::Type _collisionEnabled);
	void AddInstance(const FIntPoint& _index, const FTransform& _transform, uint8 _tileStateMask, ETileType _tileType);
	void RemoveInstance(const FIntPoint& _index);
	virtual void ClearInstances() override;
protected:
	UPROPERTY(VisibleAnywhere, Category = "Property", Meta = (DisplayName = "Instance Indices"))
	TArray<FIntPoint> m_InstanceIndices;

	UPROPERTY(VisibleAnywhere, Category = "Internal")
	bool m_bIsColorBasedOnTileType;

private:
	FColor _GetColorFromState(uint8 _tileStateMask);
	FColor _GetColorFromType(ETileType _tileType);
	
	
};
