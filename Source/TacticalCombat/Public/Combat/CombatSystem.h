// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Dispatcher/CombatDispatcher.h"
#include "CombatSystem.generated.h"

class AGrid;
class AUnit;
struct FSpellCast;
enum class ESpellType: uint8;


UCLASS()
class TACTICALCOMBAT_API ACombatSystem : public AActor, public CombatDispatcher
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

	TArray<FIntPoint> GetSpellRangeIndices(const FIntPoint& _originIndex, const FSpellCast& _cast) const;
	void CastSpell(ESpellType _spellType, const FIntPoint& _originIndex, const TArray<FIntPoint>& _targetIndices);

	bool HasLineOfSight(const FIntPoint& _originIndex, const FIntPoint& _targetIndex, const FSpellCast& _cast) const;
	TArray<FIntPoint> GetIndicesWithLineOfSight(const FIntPoint& _originIndex, const TArray<FIntPoint>& _indices, const FSpellCast& _cast) const;

public:
#pragma region Properties
	UPROPERTY(EditAnywhere, Category = "Property", Meta = (DisplayName = "Grid"))
	TWeakObjectPtr<AGrid> m_Grid;
#pragma endregion

protected:
#pragma region Components
	UPROPERTY(VisibleAnywhere, Category = "Component", Meta = (DisplayName = "Root"))
	TObjectPtr<USceneComponent> m_SceneComponent;
#pragma endregion

private:
#pragma region Internals
	UPROPERTY(VisibleAnywhere, Category = "Internal", Meta = (DisplayName = "Units In Combat"))
	TArray<TWeakObjectPtr<AUnit>> m_UnitsInCombat;
#pragma endregion

private:
	void _SetUnitIndexOnGridWithNotify(AUnit* _pUnit, const FIntPoint& _index, bool _bIsForce = false);

#pragma region Event Handlers
	UFUNCTION()
	void _HandleGridGenerated();

	UFUNCTION()
	void _HandleTileDataUpdated(const FIntPoint& _index);

	UFUNCTION()
	void _HandleUnitResearchedNewTile(AUnit* const _pUnit, const FIntPoint& _index);
#pragma endregion
};

