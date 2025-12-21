// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Unit.generated.h"

enum class ETacticalUnitType : uint8;
class USceneComponent;
class USkeletalMeshComponent;
class USkeletalMesh;
class UAnimInstance;

UCLASS()
class TACTICALCOMBAT_API AUnit : public AActor
{
	GENERATED_BODY()

	
public:	
	// Sets default values for this actor's properties
	AUnit();
	
	virtual void OnConstruction(const FTransform& _transform) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	void InitializeUnit(ETacticalUnitType _type);

public:
	FORCEINLINE const FIntPoint& GetIndex() const { return m_Index;}

	FORCEINLINE void SetUnitType(ETacticalUnitType _type) { m_UnitType = _type; }
	FORCEINLINE void SetIndex(const FIntPoint& _index) { m_Index = _index; }
	

protected:

#pragma region Properties
	UPROPERTY(EditAnywhere, Category = "Property", Meta = (DisplayName = "Unit Type"))
	ETacticalUnitType m_UnitType;
#pragma endregion


#pragma region Components
	UPROPERTY(VisibleAnywhere, Category = "Component", Meta = (DisplayName = "Scene Component"))
	TObjectPtr<USceneComponent> m_SceneComponent;

	UPROPERTY(VisibleAnywhere, Category = "Component", Meta = (DisplayName = "Skeletal Mesh Component"))
	TObjectPtr<USkeletalMeshComponent> m_SkeletalMeshComponent;
#pragma endregion

#pragma region Internals
	UPROPERTY(VisibleInstanceOnly, Category = "Internals", Meta = (DisplayName = "Index"))
	FIntPoint m_Index;
#pragma endregion
};
