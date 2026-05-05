// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../../Dispatcher/AnimatedShapeDispatcher.h"
#include "AnimatedShape.generated.h"

class UTimelineComponent;

UCLASS()
class TACTICALCOMBAT_API AAnimatedShape : public AActor, public AnimatedShapeDispatcher
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAnimatedShape();

protected:
	virtual void BeginPlay() override;
	
public:
	struct FPostSpawnInitializeParam
	{
		UStaticMesh* const pMesh;
		UMaterialInstance* const pMaterial;
		const FLinearColor& Color;
		const FTransform& OriginTransform;
		const FTransform& TargetTransform;
		float PlayRate;
	};
	void PostSpawnInitialize(const FPostSpawnInitializeParam& _param);


private:
    void _InitializeAnimatedShapeComponent();
    void _InitializeTimelineComponent();
	
protected:
#pragma region Properties
	UPROPERTY(EditAnywhere, Category = "Property", Meta = (DisplayName = "Mesh"))
	TObjectPtr<UStaticMesh> m_Mesh;

	UPROPERTY(EditAnywhere, Category = "Property", Meta = (DisplayName = "Material Instance"))
	TObjectPtr<UMaterialInstance> m_Material;

	UPROPERTY(EditAnywhere, Category = "Property", Meta = (DisplayName = "Color"))
	FLinearColor m_Color;

	UPROPERTY(EditAnywhere, Category = "Property", Meta = (DisplayName = "Movement Curve"))
	TObjectPtr<UCurveFloat> m_MovementCurve;

	UPROPERTY(EditAnywhere, Category = "Property", Meta = (DisplayName = "Height Curve"))
	TObjectPtr<UCurveFloat> m_HeightCurve;

	UPROPERTY(EditAnywhere, Category = "Property", Meta = (DisplayName = "Play Rate"))
	float m_PlayRate = 1.0f;
#pragma endregion

#pragma region Components
	UPROPERTY(VisibleAnywhere, Category = "Component", Meta = (DisplayName = "Scene Component"))
	TObjectPtr<USceneComponent> m_SceneComponent;
	
	UPROPERTY(VisibleAnywhere, Category = "Component", Meta = (DisplayName = "Animated Shape Component"))
	TObjectPtr<UStaticMeshComponent> m_AnimatedShapeComponent;

	UPROPERTY(VisibleAnywhere, Category = "Component", Meta = (DisplyName = "Timeline Component"))
	TObjectPtr<UTimelineComponent> m_TimelineComponent;	
#pragma endregion

#pragma region Internals
	UPROPERTY(EditInstanceOnly, Category = "Internal", Meta = (DisplayName = "Origin Transform"))
	FTransform m_OriginTransform;

	UPROPERTY(EditInstanceOnly, Category = "Internal", Meta = (DisplayName = "Target Transform"))
	FTransform m_TargetTransform;
#pragma endregion

private:
#pragma region Timeline Event Handler
	UFUNCTION()
	void _HandleMovementCurveUpdated(float _value);

	UFUNCTION()
	void _HandleHeightCurveUpdated(float _value);

	UFUNCTION()
	void _HandleTimelineFinished();
#pragma endregion

};