// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grid/Grid.h"
#include "PlayerActions.generated.h"

class UAbstractAction;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnSelectedActionsChanged, const UAbstractAction* const, const UAbstractAction* const);

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

#pragma region Delegate
	FOnSelectedActionsChanged OnSelectedActionsChanged;
#pragma endregion

#pragma region Getter
	FORCEINLINE const FIntPoint& GetHoveredTileIndex() const { return m_HoveredTileIndex; }
	FORCEINLINE const FIntPoint& GetSelectedTileIndex() const { return m_SelectedTileIndex; }
	FORCEINLINE AGrid* const GetGrid() const { return m_Grid.Get(); }
	FORCEINLINE UAbstractAction* const GetLeftClickSelectAction() const { return m_LeftClickSelectAction.Get(); }
	FORCEINLINE UAbstractAction* const GetRightClickSelectAction() const { return m_RightClickSelectAction.Get(); }
	FORCEINLINE bool IsLeftClickDown() const { return m_bIsLeftClickDown; }
	FORCEINLINE bool IsRightClickDown() const { return m_bIsRightClickDown; }
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
	
	UPROPERTY(VisibleInstanceOnly, Category = "Internal", Meta = (DisplayName = "Grid"))
	TWeakObjectPtr<AGrid> m_Grid;

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

	void _OnHoveredTileChanged();

	UFUNCTION()
	void _OnLeftClickPressed();

	UFUNCTION()
	void _OnLeftClickReleased();

	UFUNCTION()
	void _OnRightClickPressed();

	UFUNCTION()
	void _OnRightClickReleased();
};