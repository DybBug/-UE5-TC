// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridMeshInst.generated.h"

class UInstancedStaticMeshComponent;
class USceneComponent;

UCLASS()
class TACTICALCOMBAT_API AGridMeshInst : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridMeshInst();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

		virtual void OnConstruction(const FTransform& Transform) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void InitializeGridMeshInst(UStaticMesh* const _pMesh, UMaterialInstance* const _pMaterial, const FColor& _color, ECollisionEnabled::Type _collisionEnabled);
	void AddInstance(const FIntPoint& _index, const FTransform& _transform);
	void RemoveInstance(const FIntPoint& _index);
	void ClearInstances();


protected:
#pragma region Components
	UPROPERTY(VisibleAnywhere, Category = "Component", Meta = (DisplayName = "Instanced StaticMesh Component"))
	TObjectPtr<UInstancedStaticMeshComponent> m_InstancedStaticMeshComponent;
#pragma endregion

#pragma region Properties
	UPROPERTY(VisibleAnywhere, Category = "Property", Meta = (DisplayName = "Instance Indices"))
	TArray<FIntPoint> m_InstanceIndices;
#pragma endregion
	

};
