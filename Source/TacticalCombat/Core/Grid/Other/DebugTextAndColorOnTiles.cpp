// Fill out your copyright notice in the Description page of Project Settings.


#include "DebugTextAndColorOnTiles.h"

#include "Components/TextRenderComponent.h"
#include "Engine/TextRenderActor.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetStringLibrary.h"
#include "TacticalCombat/Core/Grid/Grid.h"
#include "TacticalCombat/Core/Grid/GridPathfinding.h"

// Sets default values
UDebugTextAndColorOnTiles::UDebugTextAndColorOnTiles()
{

}

// Called when the game starts or when spawned
void UDebugTextAndColorOnTiles::Initialize()
{
	m_Grid = Cast<AGrid>(UGameplayStatics::GetActorOfClass(GetWorld(), AGrid::StaticClass()));
	m_Grid->OnTileDataUpdated.AddUObject(this, &UDebugTextAndColorOnTiles::UpdateTextOnTile);
	m_Grid->OnGridDestroyed.AddUObject(this, &UDebugTextAndColorOnTiles::ClearAllTextActors);
	m_Grid->GetGridPathfinding()->OnPathfindingNodeUpdated.AddUObject(this, &UDebugTextAndColorOnTiles::UpdateStateOnTile);
	m_Grid->GetGridPathfinding()->OnPathfindingNodeCleared.AddUObject(this, &UDebugTextAndColorOnTiles::UpdateStateOnAllTiles);
	m_Grid->GetGridPathfinding()->OnPathfindingNodeUpdated.AddUObject(this, &UDebugTextAndColorOnTiles::UpdateTextOnTile);
	m_Grid->GetGridPathfinding()->OnPathfindingNodeCleared.AddUObject(this, &UDebugTextAndColorOnTiles::UpdateTextOnAllTiles);
}

ATextRenderActor* UDebugTextAndColorOnTiles::GetTextActor(const FIntPoint& _index)
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

void UDebugTextAndColorOnTiles::DestroyTextActor(const FIntPoint& _index)
{
	TObjectPtr<ATextRenderActor>* pTextRenderActor = m_SpawnedTextMap.Find(_index);
	if (pTextRenderActor != nullptr && IsValid(pTextRenderActor->Get()))
	{
		(*pTextRenderActor)->Destroy();
		m_SpawnedTextMap.Remove(_index);
	}
}

void UDebugTextAndColorOnTiles::ClearAllTextActors()
{
	for (auto it = m_SpawnedTextMap.CreateIterator(); it; ++it)
	{
		TObjectPtr<ATextRenderActor>& actor = it.Value();
		actor->Destroy();
	}
	m_SpawnedTextMap.Empty();
}

void UDebugTextAndColorOnTiles::UpdateTextOnTile(const FIntPoint& _index)
{
	if (!IsShowDebugText()) return;
		
	const TMap<FIntPoint, FTileData>&  gridTileMap = m_Grid->GetGridTileMap();

	const FTileData* pTileData = gridTileMap.Find(_index);
	if (pTileData != nullptr)
	{
		int longestTextLength = 0;
		
		TArray<FString> textes;

		// 타일 인덱스 텍스트
		if (HasTileDebugFlag(ETileDebugFlags::TileIndices))
		{
			FString debugStr = FString::Printf(TEXT("%d, %d"), _index.X, _index.Y);
			longestTextLength = FMath::Max(longestTextLength, debugStr.Len());
			textes.Add(debugStr);
		}

		// 타일 타입 텍스트
		if (HasTileDebugFlag(ETileDebugFlags::TileType))
		{
			if (pTileData->Type != ETileType::Normal && pTileData->Type != ETileType::None)
			{
				FString debugStr = FString::Printf(TEXT("%s"), *StaticEnum<ETileType>()->GetNameStringByValue((uint8)pTileData->Type));
				longestTextLength = FMath::Max(longestTextLength, debugStr.Len());
				textes.Add(debugStr);
			}
		}

		// 타일 경로 텍스트
		if (const FPathfindingNode* pPathfindingNode = m_Grid->GetGridPathfinding()->FindPathFindingNode(_index))
		{
			if (HasTileDebugFlag(ETileDebugFlags::CostToEnterTile))
			{
				FString debugStr =  FString::Printf(TEXT("Enter: %d"),  pPathfindingNode->CostToEnterTile);
				longestTextLength = FMath::Max(longestTextLength, debugStr.Len());
				textes.Add(debugStr);				
			}

			if (HasTileDebugFlag(ETileDebugFlags::MinCostToTarget))
			{
				if (pPathfindingNode->MinimumCostToTarget != DEFAULT_COST)
				{
					FString debugStr = FString::Printf(TEXT("Min: %d"),  pPathfindingNode->MinimumCostToTarget);
					longestTextLength = FMath::Max(longestTextLength, debugStr.Len());
					textes.Add(debugStr);	
				}
			}
			
			if (HasTileDebugFlag(ETileDebugFlags::CostFromStart))
			{
				if (pPathfindingNode->CostFromStart != DEFAULT_COST)
				{
					FString debugStr = FString::Printf(TEXT("Start: %d"),  pPathfindingNode->CostFromStart);
					longestTextLength = FMath::Max(longestTextLength, debugStr.Len());
					textes.Add(debugStr);	
				}
			}

			if (HasTileDebugFlag(ETileDebugFlags::SortOrder))
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

		// 텍스트 머지
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

void UDebugTextAndColorOnTiles::UpdateTextOnAllTiles()
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

void UDebugTextAndColorOnTiles::UpdateStateOnTile(const FIntPoint& _index)
{
	if (HasTileDebugFlag(ETileDebugFlags::DiscoveredTiles))
	{
		int32 discoveredNodeIndex = m_Grid->GetGridPathfinding()->FindDiscoveredNodeIndex(_index);
		if (discoveredNodeIndex != INDEX_NONE)
		{
			m_Grid->AddStateToTile(_index, ETileStateFlags::Discovered);
		}
		else
		{
			m_Grid->RemoveStateFromTile(_index, ETileStateFlags::Discovered);
		}
	}
	else
	{
		m_Grid->RemoveStateFromTile(_index, ETileStateFlags::Discovered);
	}
	

	if (HasTileDebugFlag(ETileDebugFlags::AnalysedTiles))
	{
		int32 analysedNodeIndex = m_Grid->GetGridPathfinding()->FindAnalysedNodeIndex(_index);
		if (analysedNodeIndex != INDEX_NONE)
		{
			m_Grid->AddStateToTile(_index, ETileStateFlags::Analyzed);
		}
		else
		{
			m_Grid->RemoveStateFromTile(_index, ETileStateFlags::Analyzed);
		}
	}
	else
	{
		m_Grid->RemoveStateFromTile(_index, ETileStateFlags::Analyzed);
	}
}

void UDebugTextAndColorOnTiles::UpdateStateOnAllTiles()
{
	if (HasTileDebugFlag(ETileDebugFlags::DiscoveredTiles) | HasTileDebugFlag(ETileDebugFlags::AnalysedTiles))
	{
		TArray<FIntPoint> tileIndices;
		m_Grid->GetGridTileMap().GetKeys(tileIndices);
		for (const FIntPoint& tileIndex : tileIndices)
		{
			UpdateStateOnTile(tileIndex);
		}
	}
	else
	{
		m_Grid->ClearStateFromTiles(ETileStateFlags::Discovered);
		m_Grid->ClearStateFromTiles(ETileStateFlags::Analyzed);
	}
}

void UDebugTextAndColorOnTiles::SetShowTileStates(bool _isShowDiscovered, bool _isShowAnalysed)
{
	SetTileDebugFlag(ETileDebugFlags::DiscoveredTiles, _isShowDiscovered);
	SetTileDebugFlag(ETileDebugFlags::AnalysedTiles, _isShowAnalysed);

	UpdateStateOnAllTiles();
}

void UDebugTextAndColorOnTiles::SetTileDebugFlag(ETileDebugFlags _flag, bool _bIsEnabled)
{
	_bIsEnabled ? AddTileDebugFlag(_flag) : RemoveTileDebugFlag(_flag);
}

void UDebugTextAndColorOnTiles::AddTileDebugFlag(ETileDebugFlags _flags)
{
	m_TileDebugMask |= static_cast<uint8>(_flags);
	UpdateTextOnAllTiles();
}

void UDebugTextAndColorOnTiles::RemoveTileDebugFlag(ETileDebugFlags _flags)
{
	m_TileDebugMask &= ~static_cast<uint8>(_flags);
	UpdateTextOnAllTiles();
}

bool UDebugTextAndColorOnTiles::HasTileDebugFlag(ETileDebugFlags _flag) const
{
	return (m_TileDebugMask & static_cast<uint8>(_flag)) != 0;
}

