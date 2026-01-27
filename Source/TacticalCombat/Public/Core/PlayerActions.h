// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerActions.generated.h"

class UAbstractAction;
class AGrid;
class AUnit;
class ACombatSystem;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnSelectedActionsChanged, const UAbstractAction* const, const UAbstractAction* const);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSelectedTileChanged, const FIntPoint&);

UCLASS()
class TACTICALCOMBAT_API APlayerActions : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerActions();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetSelectedActionWithNotify(const TSubclassOf<UAbstractAction>& _leftClickActionClass, const TSubclassOf<UAbstractAction>& _rightClickActionClass);
	void SelectTileAndUnit(const FIntPoint& _index, bool _isForce = false);

#pragma region Events
	FOnSelectedActionsChanged OnSelectedActionsChanged;
	FOnSelectedTileChanged OnSelectedTileChanged;
#pragma endregion

#pragma region Getter
	FORCEINLINE const FIntPoint& GetHoveredTileIndex() const { return m_HoveredTileIndex; }
	FORCEINLINE const FIntPoint& GetSelectedTileIndex() const { return m_SelectedTileIndex; }
	FORCEINLINE AGrid* const GetGrid() const { return m_Grid.Get(); }
	FORCEINLINE ACombatSystem* const GetCombatSystem() const { return m_CombatSystem.Get(); }
	FORCEINLINE UAbstractAction* const GetLeftClickSelectAction() const { return m_LeftClickSelectAction.Get(); }
	FORCEINLINE UAbstractAction* const GetRightClickSelectAction() const { return m_RightClickSelectAction.Get(); }
	FORCEINLINE bool IsLeftClickDown() const { return m_bIsLeftClickDown; }
	FORCEINLINE bool IsRightClickDown() const { return m_bIsRightClickDown; }
	FORCEINLINE AUnit* const GetSelectedUnit() const { return m_SelectedUnit.Get(); }
#pragma endregion

#pragma region Setter
	FORCEINLINE void SetSelectedTileIndex(const FIntPoint& _index) { m_SelectedTileIndex = _index; }
#pragma endregion

protected:
#pragma region Property
	UPROPERTY(EditAnywhere, Category = "Property", Meta = (DisplayName = "Left Click Select Action Class"))
	TSubclassOf<UAbstractAction> m_LeftClickSelectActionClass;

	UPROPERTY(EditAnywhere, Category = "Property", Meta = (DisplayName = "Right Click Select Action Class"))
	TSubclassOf<UAbstractAction> m_RightClickSelectActionClass;
#pragma endregion

#pragma region Internal
	UPROPERTY(VisibleInstanceOnly, Category = "Internal", Meta = (DisplayName = "Hovered Tile Index"))
	FIntPoint m_HoveredTileIndex;

	UPROPERTY(VisibleInstanceOnly, Category = "Internal", Meta = (DisplayName = "Selected Tile Index"))
	FIntPoint m_SelectedTileIndex;

	UPROPERTY(VisibleInstanceOnly, Category = "Internal", Meta = (DisplayName = "Hovered Unit"))
	TWeakObjectPtr<AUnit> m_HoveredUnit;

	UPROPERTY(VisibleInstanceOnly, Category = "Internal", Meta = (DisplayName = "Selected Unit"))
	TWeakObjectPtr<AUnit> m_SelectedUnit;
	
	UPROPERTY(VisibleInstanceOnly, Category = "Internal", Meta = (DisplayName = "Grid"))
	TWeakObjectPtr<AGrid> m_Grid;

	UPROPERTY(VisibleInstanceOnly, Category = "Internal", Meta = (DisplayName = "Combat System"))
	TWeakObjectPtr<ACombatSystem> m_CombatSystem;

	UPROPERTY(VisibleInstanceOnly, Category = "Internal", Meta = (DisplayName = "Left Click Select Action"))
	TObjectPtr<UAbstractAction> m_LeftClickSelectAction;

	UPROPERTY(VisibleInstanceOnly, Category = "Internal", Meta = (DisplayName = "Right Click Select Action"))
	TObjectPtr<UAbstractAction> m_RightClickSelectAction;

	UPROPERTY(VisibleInstanceOnly, Category = "Internal", Meta = (DisplayName = "Is Left Click Down"))
	bool m_bIsLeftClickDown;

	UPROPERTY(VisibleINSTANCEONLY, Category = "Internal", Meta = (DisplayName = "Is Right Click Down"))
	bool m_bIsRightClickDown;
#pragma endregion


private:
	void _UpdateHoveredTile();
	void _UpdateHoveredUnit();
	AUnit* _FindUnitUnderCursor(uint8 _playerIndex);

	void _OnHoveredTileChanged();

#pragma region Event Handlers
	UFUNCTION()
	void _HandleLeftClickPressed();

	UFUNCTION()
	void _HandleLeftClickReleased();

	UFUNCTION()
	void _HandleRightClickPressed();

	UFUNCTION()
	void _HandleRightClickReleased();

	UFUNCTION()
	void _HandleGridGenerated();

	UFUNCTION()
	void _HandleTileDataUpdated(const FIntPoint& _index);

	UFUNCTION()
	void _HandleUnitGridIndexChanged(AUnit* const _pUnit);
#pragma endregion
};