// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CombatSystem.generated.h"

class AGrid;
class AUnit;

UCLASS()
class TACTICALCOMBAT_API ACombatSystem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACombatSystem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	void AddUnitInCombat(AUnit* _pUnit, const FIntPoint& _index);
	void RemoveUnitFromCombat(AUnit* _pUnit, bool _bIsUnitDestroyed);

public:
#pragma region Properties
	UPROPERTY(EditAnywhere, Category = "Property", Meta = (DisplayName = "Grid"))
	TWeakObjectPtr<AGrid> m_Grid;
#pragma endregion

#pragma region Components
	UPROPERTY(VisibleAnywhere, Category = "Component", Meta = (DisplayName = "Root"))
	TObjectPtr<USceneComponent> m_SceneComponent;
#pragma endregion

#pragma region Internals
	UPROPERTY(VisibleAnywhere, Category = "Internal", Meta = (DisplayName = "Units In Combat"))
	TArray<TWeakObjectPtr<AUnit>> m_UnitsInCombat;
#pragma endregion

private:
	void _SetUnitIndexOnGrid(AUnit* _pUnit, const FIntPoint& _index);

	UFUNCTION()
	void _OnGridGenerated();

	UFUNCTION()
	void _OnTileDataUpdated(const FIntPoint& _index);
};
