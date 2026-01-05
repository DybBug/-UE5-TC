// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Table/Rows/UnitTableRow.h"
#include "Unit.generated.h"

enum class ETacticalUnitType : uint8;
enum class EUnitAnimationState: uint8;
class UTimelineComponent;
class USceneComponent;
class USkeletalMeshComponent;
class USkeletalMesh;
class UAnimInstance;
class AGrid;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnUnitResearchedNewTile, AUnit* const, const FIntPoint&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnUnitFinishedWalking, AUnit* const);

UCLASS()
class TACTICALCOMBAT_API AUnit : public AActor
{
	GENERATED_BODY()

public:
	static AUnit* Spawn(UWorld* _pWorld, ETacticalUnitType _type, AGrid* _pGrid);
	
public:	
	// Sets default values for this actor's properties
	AUnit();
	
	virtual void OnConstruction(const FTransform& _transform) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
#pragma region Events
	FOnUnitResearchedNewTile OnUnitResearchedNewTile;
	FOnUnitFinishedWalking OnUnitFinishedWalking;
#pragma endregion 

public:
	void InitializeUnit(ETacticalUnitType _type, AGrid* _pGrid);
	void UpdateVisual();

	void FollowPath(const TArray<FIntPoint>& _path);
	
public:
	FORCEINLINE const FIntPoint& GetIndex() const { return m_Index;}
	FORCEINLINE const FUnitTableRow& GetUnitData() const { return m_UnitData; }

	FORCEINLINE void SetUnitType(ETacticalUnitType _type) { m_UnitType = _type; }
	FORCEINLINE void SetMoveDurationPerTile(float _value) { m_MoveDurationPerTile = _value; }
	FORCEINLINE void SetIndex(const FIntPoint& _index) { m_Index = _index; }
	FORCEINLINE void SetIsHovered(bool _bIsHovered);
	FORCEINLINE void SetIsSelected(bool _bIsSelected);
protected:

#pragma region Properties
	UPROPERTY(EditAnywhere, Category = "Property", Meta = (DisplayName = "Unit Type"))
	ETacticalUnitType m_UnitType;

	UPROPERTY(EditAnywhere, Category = "Property", Meta = (DisplayName = "Moving Speed"))
	float m_MoveDurationPerTile;
	
	UPROPERTY(EditAnywhere, Category = "Property", Meta = (DisplayName = "Unit Location Curve"))
	TObjectPtr<UCurveFloat> m_UnitLocationCurve;

	UPROPERTY(EditAnywhere, Category = "Property", Meta = (DisplayName = "Unit Rotation Curve"))
	TObjectPtr<UCurveFloat> m_UnitRotationCurve;

	UPROPERTY(EditAnywhere, Category = "Property", Meta = (DisplayName = "Unit Jump Curve"))
	TObjectPtr<UCurveFloat> m_UnitJumpCurve;
#pragma endregion

#pragma region Components
	UPROPERTY(VisibleAnywhere, Category = "Component", Meta = (DisplayName = "Scene Component"))
	TObjectPtr<USceneComponent> m_SceneComponent;

	UPROPERTY(VisibleAnywhere, Category = "Component", Meta = (DisplayName = "Skeletal Mesh Component"))
	TObjectPtr<USkeletalMeshComponent> m_SkeletalMeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "Component", Meta = (DisplayName = "Unit Movement Timeline Component"))
	TObjectPtr<UTimelineComponent> m_UnitMovementTimelineComponent;
#pragma endregion

#pragma region Internals
	UPROPERTY(VisibleInstanceOnly, category = "Internals", Meta = (DisplayName = "Grid"))
	TWeakObjectPtr<AGrid> m_Grid;
	
	UPROPERTY(VisibleInstanceOnly, Category = "Internals", Meta = (DisplayName = "Index"))
	FIntPoint m_Index;

	UPROPERTY(VisibleInstanceOnly, Category = "Internals", Meta = (DisplayName = "Is Hovered"))
	bool m_bIsHovered;

	UPROPERTY(VisibleInstanceOnly, Category = "Internals", Meta = (DisplayName = "Is Selected"))
	bool m_bIsSelected;

	UPROPERTY(VisibleInstanceOnly, Category = "Internals", Meta = (DisplayName = "Unit Data"))
	FUnitTableRow m_UnitData;

	UPROPERTY(VisibleInstanceOnly, Category = "Internals", Meta = (DisplayName = "Current Path To Follow"))
	TArray<FIntPoint> m_CurrentPathToFollow;

	FTransform m_PreviousTransform;
	FTransform m_NextTransform;

#pragma endregion

private:
	void _SetAnimationState(EUnitAnimationState _state);

#pragma region Timeline Event Handles
	UFUNCTION()
	void _HandleUnitLocationCurveUpdated(float _value);

	UFUNCTION()
	void _HandleUnitRotationCurveUpdated(float _value);

	UFUNCTION()
	void _HandleUnitJumpCurveUpdated(float _value);
	
	UFUNCTION()
	void _HandleUnitMovementTimelineFinishedWithNotify();
#pragma endregion
};

