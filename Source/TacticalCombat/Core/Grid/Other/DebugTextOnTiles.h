// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DebugTextOnTiles.generated.h"

class AGrid;
class ATextRenderActor;
enum class ETileDebugFlags : uint8;

UCLASS()
class TACTICALCOMBAT_API UDebugTextOnTiles : public UObject
{
	GENERATED_BODY()
	
public:
	UDebugTextOnTiles();
	
	void Initialize();	

	ATextRenderActor* GetTextActor(const FIntPoint& _index);
	void DestroyTextActor(const FIntPoint& _index);

	UFUNCTION()
	void ClearAllTextActors();

	UFUNCTION()
	void UpdateTextOnTile(const FIntPoint& _index);

	UFUNCTION()
	void UpdateTextOnAllTiles();
	
	void AddTileDebugFlag(ETileDebugFlags _flags);
	void RemoveTileDebugFlag(ETileDebugFlags _flags);
	bool HasTileDebugFlag(ETileDebugFlags flag) const;
	
	FORCEINLINE bool IsShowDebugText() const { return static_cast<uint8>(m_TileDebugMask) != 0; }

protected:
#pragma region Internal
	UPROPERTY(VisibleInstanceOnly, Category = "Internal", Meta = (DisplayName = "Spawned Text Map"))
	TMap<FIntPoint, TObjectPtr<ATextRenderActor>> m_SpawnedTextMap;

	UPROPERTY(VisibleInstanceOnly, Category = "Internal", Meta = (DisplayName = "Grid"))
	TObjectPtr<AGrid> m_Grid;

	UPROPERTY(VisibleInstanceOnly, Category = "Internal", Meta = (DisplayName = "Tile Debug Mask"))
	uint8 m_TileDebugMask;
#pragma endregion

private:
	void _ReUpdateAllTextsAfterDelay(const FIntPoint& _index);

};
