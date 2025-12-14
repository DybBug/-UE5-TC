// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Unit.generated.h"

enum class EUnitType : uint8;
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

protected:

#pragma region Properties
	UPROPERTY(EditAnywhere, Category = "Property", Meta = (DisplayName = "Unit Type"))
	EUnitType m_UnitType;

	UPROPERTY(EditAnywhere, Category = "Property", Meta = (DisplayName = "Unit Types To Skeletal Mesh"))
	TMap<EUnitType, TObjectPtr<USkeletalMesh>> m_UnitTypesToSkeletalMesh;

	UPROPERTY(EditAnywhere, Category = "Property", Meta = (DisplayName = "Unit TypesTo Anim Instance Class"))
	TMap<EUnitType, TSubclassOf<UAnimInstance>> m_UnitTypesToAnimInstanceClass;
#pragma endregion


#pragma region Components
	UPROPERTY(VisibleAnywhere, Category = "Component", Meta = (DisplayName = "Scene Component"))
	TObjectPtr<USceneComponent> m_SceneComponent;

	UPROPERTY(VisibleAnywhere, Category = "Component", Meta = (DisplayName = "Skeletal Mesh Component"))
	TObjectPtr<USkeletalMeshComponent> m_SkeletalMeshComponent;
#pragma endregion
};
