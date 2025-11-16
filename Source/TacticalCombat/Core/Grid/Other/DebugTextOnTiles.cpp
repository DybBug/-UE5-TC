// Fill out your copyright notice in the Description page of Project Settings.


#include "DebugTextOnTiles.h"

#include "Components/TextRenderComponent.h"
#include "Engine/TextRenderActor.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetStringLibrary.h"
#include "TacticalCombat/Core/Grid/Grid.h"
#include "TacticalCombat/Core/Grid/GridPathfinding.h"

// Sets default values
UDebugTextOnTiles::UDebugTextOnTiles()
{

}

// Called when the game starts or when spawned
void UDebugTextOnTiles::Initialize()
{
	m_Grid = Cast<AGrid>(UGameplayStatics::GetActorOfClass(GetWorld(), AGrid::StaticClass()));
	m_Grid->OnTileDataUpdated.AddUObject(this, &UDebugTextOnTiles::UpdateTextOnTile);
	m_Grid->OnGridDestroyed.AddUObject(this, &UDebugTextOnTiles::ClearAllTextActors);
	m_Grid->GetGridPathfinding()->OnPathfindingNodeUpdated.AddUObject(this, &UDebugTextOnTiles::_ReUpdateAllTextsAfterDelay);
	m_Grid->GetGridPathfinding()->OnPathfindingNodeCleared.AddUObject(this, &UDebugTextOnTiles::ClearAllTextActors);
}

ATextRenderActor* UDebugTextOnTiles::GetTextActor(const FIntPoint& _index)
{
	TObjectPtr<ATextRenderActor>* pTextRenderActor = m_SpawnedTextMap.Find(_index);
	if (pTextRenderActor == nullptr || !IsValid(pTextRenderActor->Get()))
	{
		ATextRenderActor* pNewText = Cast<ATextRenderActor>(GetWorld()->SpawnActor(ATextRenderActor::StaticClass(), &FTransform::Identity));
		pNewText->SetActorTickEnabled(false);
		pNewText->SetActorEnableCollision(false);
		pNewText->GetTextRender()->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
		pNewText->GetTextRender()->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextCenter);
		m_SpawnedTextMap.Add(_index, TObjectPtr<ATextRenderActor>(pNewText));
		return pNewText;
	}
	
	return pTextRenderActor->Get();	
}

void UDebugTextOnTiles::DestroyTextActor(const FIntPoint& _index)
{
	TObjectPtr<ATextRenderActor>* pTextRenderActor = m_SpawnedTextMap.Find(_index);
	if (pTextRenderActor != nullptr && IsValid(pTextRenderActor->Get()))
	{
		(*pTextRenderActor)->Destroy();
		m_SpawnedTextMap.Remove(_index);
	}
}

void UDebugTextOnTiles::ClearAllTextActors()
{
	for (auto it = m_SpawnedTextMap.CreateIterator(); it; ++it)
	{
		TObjectPtr<ATextRenderActor>& actor = it.Value();
		actor->Destroy();
	}
	m_SpawnedTextMap.Empty();
}

void UDebugTextOnTiles::UpdateTextOnTile(const FIntPoint& _index)
{
	if (!IsShowDebugText()) return;
		
	const TMap<FIntPoint, FTileData>&  gridTileMap = m_Grid->GetGridTileMap();

	const FTileData* pTileData = gridTileMap.Find(_index);
	if (pTileData != nullptr)
	{
		int longestTextLength = 0;
		
		TArray<FString> textes;
		
		if ((m_TileDebugMask & static_cast<uint8>(ETileDebugFlags::TileIndices)) != 0)
		{
			FString debugStr = FString::Printf(TEXT("%d, %d"), _index.X, _index.Y);
			longestTextLength = FMath::Max(longestTextLength, debugStr.Len());
			textes.Add(debugStr);
		}
		
		if (const FPathfindingNode* pPathfindingNode = m_Grid->GetGridPathfinding()->FindPathFindingNode(_index))
		{
			if ((m_TileDebugMask & static_cast<uint8>(ETileDebugFlags::CostToEnterTile)) != 0)
			{
				FString debugStr =  FString::Printf(TEXT("Enter: %d"),  pPathfindingNode->CostToEnterTile);
				longestTextLength = FMath::Max(longestTextLength, debugStr.Len());
				textes.Add(debugStr);				
			}

			if ((m_TileDebugMask & static_cast<uint8>(ETileDebugFlags::MinCostToTarget)) != 0)
			{
				if (pPathfindingNode->MinimumCostToTarget != DEFAULT_COST)
				{
					FString debugStr = FString::Printf(TEXT("Min: %d"),  pPathfindingNode->MinimumCostToTarget);
					longestTextLength = FMath::Max(longestTextLength, debugStr.Len());
					textes.Add(debugStr);	
				}
			}
			
			if ((m_TileDebugMask & static_cast<uint8>(ETileDebugFlags::CostFromStart)) != 0)
			{
				if (pPathfindingNode->CostFromStart != DEFAULT_COST)
				{
					FString debugStr = FString::Printf(TEXT("Start: %d"),  pPathfindingNode->CostFromStart);
					longestTextLength = FMath::Max(longestTextLength, debugStr.Len());
					textes.Add(debugStr);	
				}
			}

			if ((m_TileDebugMask & static_cast<uint8>(ETileDebugFlags::SortOrder)) != 0)
			{
				int32 discoveredNodeIndex = m_Grid->GetGridPathfinding()->FindDiscoveredNodeIndex(_index);
				 if (discoveredNodeIndex != INDEX_NONE)
				 {
				 	int32 discoveredNodeSortingCost = m_Grid->GetGridPathfinding()->FindDiscoveredNodeSortingCost(discoveredNodeIndex);
				 	FString debugStr = FString::Printf(TEXT("Sort: %d (%d)"), discoveredNodeIndex, discoveredNodeSortingCost);
				 	longestTextLength = FMath::Max(longestTextLength, debugStr.Len());
				 	textes.Add(debugStr);
				 }
			}
		}		
		
		if (textes.Num() > 0)
		{
			ATextRenderActor* const pTextActor =  GetTextActor(_index);
		
			FString separator = TEXT("\n");
			pTextActor->GetTextRender()->SetText(FText::FromString(UKismetStringLibrary::JoinStringArray(textes, separator)));
		
			if (!AGrid::IsWalkableTile(pTileData->Type))
			{
				DestroyTextActor(_index);
			}

			FTransform transform;
			transform.SetLocation(pTileData->Transform.GetLocation() + FVector(0.0f, 0.0f, 1.0f));
			transform.SetRotation(FRotator(90.0f, 180.0f, 0.0f).Quaternion());
			transform.SetScale3D(FVector(m_Grid->GetTileSize().Y / (longestTextLength * 30.0f)));
			pTextActor->SetActorTransform(transform);
			return;
		}
	}
	
	DestroyTextActor(_index);
}

void UDebugTextOnTiles::UpdateTextOnAllTiles()
{
	if (!IsShowDebugText())
	{
		ClearAllTextActors();
		return;
	}

	TArray<FIntPoint> tileIndices;
	m_Grid->GetGridTileMap().GetKeys(tileIndices);
	for (const FIntPoint& tileIndex : tileIndices)
	{
		UpdateTextOnTile(tileIndex);
	}	
}

void UDebugTextOnTiles::AddTileDebugFlag(ETileDebugFlags _flags)
{
	m_TileDebugMask |= static_cast<uint8>(_flags);
	UpdateTextOnAllTiles();
}

void UDebugTextOnTiles::RemoveTileDebugFlag(ETileDebugFlags _flags)
{
	m_TileDebugMask &= ~static_cast<uint8>(_flags);
	UpdateTextOnAllTiles();
}

bool UDebugTextOnTiles::HasTileDebugFlag(ETileDebugFlags flag) const
{
	return (m_TileDebugMask & static_cast<uint8>(flag)) != 0;
}

#pragma region Private Methods
void UDebugTextOnTiles::_ReUpdateAllTextsAfterDelay(const FIntPoint& _index)
{
	FLatentActionInfo latentInfo;
	latentInfo.CallbackTarget = this;
	latentInfo.ExecutionFunction = TEXT("UpdateTextOnAllTiles");
	latentInfo.Linkage = 0;
	latentInfo.UUID = __LINE__;
	UKismetSystemLibrary::RetriggerableDelay(GetWorld(), 0.1f, latentInfo);
}
#pragma endregion

