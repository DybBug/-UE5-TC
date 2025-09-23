// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TacticalCombat/Misc/Enums.h"
#include "GridModifier.generated.h"

enum class EGridShape: uint8;
enum class ETileType: uint8;

UCLASS()
class TACTICALCOMBAT_API AGridModifier : public AActor
{
	GENERATED_BODY()

static TMap<ETileType, FColor> TileColorMap;

	
public:	
	// Sets default values for this actor's properties
	AGridModifier();

	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	FORCEINLINE ETileType GetType() const { return m_Type; }

protected:
#pragma region Components
	UPROPERTY(VisibleAnywhere, Category = "Component", Meta = (DisplayName = "Root"))
	TObjectPtr<USceneComponent> m_SceneComponent;
	
	UPROPERTY(VisibleAnywhere, Category = "Component", Meta = (DisplayName = "Static Mesh"))
	TObjectPtr<UStaticMeshComponent> m_StaticMeshComponent;
#pragma endregion

#pragma region Properties
	UPROPERTY(EditAnywhere, Category = "Property", Meta = (DisplayName = "Shape"))
	EGridShape m_Shape = EGridShape::Square;

	UPROPERTY(EditAnywhere, Category = "Property", Meta = (DisplayName = "Shape"))
	ETileType m_Type;
#pragma endregion
};
