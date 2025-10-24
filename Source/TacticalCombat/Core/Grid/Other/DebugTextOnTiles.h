// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DebugTextOnTiles.generated.h"

class AGrid;
class ATextRenderActor;

UCLASS()
class TACTICALCOMBAT_API UDebugTextOnTiles : public UObject
{
	GENERATED_BODY()
	
public:
	UDebugTextOnTiles();
	
	void Initialize();
	
	
	UFUNCTION()
	ATextRenderActor* GetTextActor(const FIntPoint& _index);

	UFUNCTION()
	void DestroyTextActor(const FIntPoint& _index);

	UFUNCTION()
	void ClearAllTextActors();

	UFUNCTION()
	void UpdateTextOnTile(const FIntPoint& _index);

#pragma region Getter
	FORCEINLINE bool IsShowTileIndices() const { return m_bIsShowTileIndices; }
#pragma endregion

#pragma region Setter
	void SetShowTileIndices(bool _bIsShow);
#pragma endregion

protected:
#pragma region Internal
	UPROPERTY(VisibleInstanceOnly, Category = "Internal", Meta = (DisplayName = "Spawned Text Map"))
	TMap<FIntPoint, TObjectPtr<ATextRenderActor>> m_SpawnedTextMap;

	UPROPERTY(VisibleInstanceOnly, Category = "Internal", Meta = (DisplayName = "Grid"))
	TObjectPtr<AGrid> m_Grid;

	UPROPERTY(VisibleInstanceOnly, Category = "Internal", Meta = (DisplayName = "Is Show Tile Indices"))
	bool m_bIsShowTileIndices;
#pragma endregion

};

