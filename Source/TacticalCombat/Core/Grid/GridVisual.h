// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridVisual.generated.h"

class AGrid;
class UInstancedStaticMeshComponent;
struct FTileData;

UCLASS()
class TACTICALCOMBAT_API AGridVisual : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridVisual();

protected:
	virtual void OnConstruction(const FTransform& Transform) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void InitializeGridVisual(AGrid* _pGrid);

	void SetZOffset(float _offset);
	FORCEINLINE float GetZOffset() const {return m_ZOffset;}
	
	void DestroyGridVisual();
	void UpdateTileVisual(const FTileData& _tileData);

	void InitializeGridMeshInst(UStaticMesh* const _pMesh, UMaterialInstance* const _pMaterial, const FColor& _color, ECollisionEnabled::Type _collisionEnabled);
	void AddInstance(const FIntPoint& _index, const FTransform& _transform, uint8 _tileStateMask);
	void RemoveInstance(const FIntPoint& _index);
	void ClearInstances();
	
protected:
#pragma region Components
	UPROPERTY(VisibleAnywhere, Category = "Component", Meta = (DisplayName = "Root"))
	TObjectPtr<USceneComponent> m_SceneComponent;

	UPROPERTY(VisibleAnywhere, Category = "Component", Meta = (DisplayName = "Instanced StaticMesh Component"))
	TObjectPtr<UInstancedStaticMeshComponent> m_InstancedStaticMeshComponent;
#pragma endregion

#pragma region Properties
	UPROPERTY(EditAnywhere, Category = "Property", Meta = (DisplayName = "Z-Offset"))
	float m_ZOffset = 2.0f;	
	
	UPROPERTY(VisibleAnywhere, Category = "Property", Meta = (DisplayName = "Instance Indices"))
	TArray<FIntPoint> m_InstanceIndices;
#pragma endregion

	
private:
	FColor _GetColorFromState(uint8 _tileStateMask);
	

};

