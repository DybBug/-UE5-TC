// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridMeshInst.h"
#include "GameFramework/Actor.h"
#include "GridVisual.generated.h"

class AGrid;
class AGridMeshInst;
struct FTileData;

UCLASS()
class TACTICALCOMBAT_API AGridVisual : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridVisual();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void InitializeGridVisual(AGrid* _pGrid);

	void SetZOffset(float _offset);
	FORCEINLINE float GetZOffset() const {return m_ZOffset;}
	
	void DestroyGridVisual();
	void UpdateTileVisual(const FTileData& _tileData);
	
protected:
#pragma region Components
	UPROPERTY(VisibleAnywhere, Category = "Component", Meta = (DisplayName = "Root"))
	TObjectPtr<USceneComponent> m_SceneComponent;

	UPROPERTY(VisibleAnywhere, Category = "Component", Meta = (DisplayName = "Grid Mesh Instance"))
	TObjectPtr<UChildActorComponent> m_ChildActorGridMeshInst;
#pragma endregion

#pragma region Properties
	UPROPERTY(EditAnywhere, Category = "Property", Meta = (DisplayName = "Z-Offset"))
	float m_ZOffset = 2.0f;

	UPROPERTY(VisibleAnywhere, Category = "Property", Meta = (DisplayName = "Grid Inst"))
	TWeakObjectPtr<AGridMeshInst> m_GridMeshInst;
#pragma endregion


};

