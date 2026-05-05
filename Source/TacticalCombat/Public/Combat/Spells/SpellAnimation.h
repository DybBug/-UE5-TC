// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Dispatcher/SpellDispatcher.h"
#include "SpellAnimation.generated.h"

class AGrid;


UCLASS()
class TACTICALCOMBAT_API ASpellAnimation : public AActor, public SpellDispatcher
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASpellAnimation();

protected:
	virtual void BeginPlay() override;

public:
	void PostSpawnInitialize(AGrid* const _pGrid, const FIntPoint& _originIndex, const TArray<FIntPoint>& _targetIndices);

public:
	void PlaySpellAnimation();
	
protected:
#pragma region Internals
	UPROPERTY(VisibleInstanceOnly, Category = "Internals", Meta = (DisplayName = "Grid"))
	TWeakObjectPtr<AGrid> m_Grid;

	UPROPERTY(VisibleInstanceOnly, Category = "Internals", Meta = (DisplayName = "Origin Index"))
	FIntPoint m_OriginIndex;

	UPROPERTY(VisibleInstanceOnly, Category = "Internals", Meta = (DisplayName = "Target Indices"))
	TArray<FIntPoint> m_TargetIndices;
#pragma endregion

private:
	FTransform _GetTileTransform(const FIntPoint& _index);

#pragma region Event Handlers
private:
	UFUNCTION()
	void HandleAnimatedShapeFinished();
#pragma endregion
	
};
