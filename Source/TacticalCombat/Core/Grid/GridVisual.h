// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TacticalCombat/Misc/Constants.h"
#include "GridVisual.generated.h"

using namespace Grid;

class AGrid;
class UGridInstancedStaticMeshComponent;
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
	void InitializeGridVisual(AGrid* _pGrid);


	
	void DestroyGridVisual();
	void UpdateTileVisual(const FTileData& _tileData);

#pragma region Getter
	FORCEINLINE bool IsTactical() const {return m_bIsTactical;}
#pragma endregion

#pragma region Setter
	void SetZOffset(float _offset);
	void SetIsTactical(bool _bIsTactical);
	FORCEINLINE float GetZOffset() const {return m_ZOffset;}
#pragma endregion
	
protected:
#pragma region Components
	UPROPERTY(VisibleAnywhere, Category = "Component", Meta = (DisplayName = "Root"))
	TObjectPtr<USceneComponent> m_SceneComponent;

	UPROPERTY(VisibleAnywhere, Category = "Component", Meta = (DisplayName = "Grid Instanced StaticMesh Component"))
	TObjectPtr<UGridInstancedStaticMeshComponent> m_GridInstancedStaticMeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "Component", Meta = (DisplayName = "Tactical Grid Instanced StaticMesh Component"))
	TObjectPtr<UGridInstancedStaticMeshComponent> m_TacticalGridInstancedStaticMeshComponent;
#pragma endregion

#pragma region Properties
	UPROPERTY(EditAnywhere, Category = "Property", Meta = (DisplayName = "Z-Offset"))
	float m_ZOffset = 2.0f;

	UPROPERTY(EditAnywhere, Category = "Property", Meta = (DisplayName = "Is Tactical"))
	bool m_bIsTactical;
#pragma endregion

#pragma region Internals
	TWeakObjectPtr<AGrid> m_Grid;
	int32 m_GridLowerZ = DEFAULT_GRID_LOWER_Z;
	bool m_bIsNeedToReGenerateTacticalOnNextEnable;
#pragma endregion

private:
	void _UpdateTileVisualTactical(const FTileData& _tileData);
	void _SetGridLowerZ(int32 _z);

};

