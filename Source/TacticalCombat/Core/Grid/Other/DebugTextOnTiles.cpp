// Fill out your copyright notice in the Description page of Project Settings.


#include "DebugTextOnTiles.h"

#include "Components/TextRenderComponent.h"
#include "Engine/TextRenderActor.h"
#include "Kismet/GameplayStatics.h"
#include "TacticalCombat/Core/Grid/Grid.h"

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
	if (!m_bIsShowTileIndices) return;
		
	const TMap<FIntPoint, FTileData>&  gridTileMap = m_Grid->GetGridTileMap();

	const FTileData* pTileData = gridTileMap.Find(_index);
	if (pTileData != nullptr)
	{
		if (!AGrid::IsWalkableTile(pTileData->Type))
		{
			DestroyTextActor(_index);
		}

		FString text = FString::Printf(TEXT("%d, %d"), _index.X, _index.Y);
		ATextRenderActor* const pTextActor =  GetTextActor(_index);
		pTextActor->GetTextRender()->SetText(FText::FromString(text));

		FTransform transform;
		transform.SetLocation(pTileData->Transform.GetLocation() + FVector(0.0f, 0.0f, 1.0f));
		transform.SetRotation(FRotator(90.0f, 180.0f, 0.0f).Quaternion());
		transform.SetScale3D(FVector(2.0f));
		pTextActor->SetActorTransform(transform);
		return;
	}
	
	DestroyTextActor(_index);
}

void UDebugTextOnTiles::SetShowTileIndices(bool _bIsShow)
{
	m_bIsShowTileIndices = _bIsShow;
	if (!m_bIsShowTileIndices)
	{
		ClearAllTextActors();
		return;
	}

	TArray<FIntPoint> keys;
	m_Grid->GetGridTileMap().GetKeys(keys);
	for (const FIntPoint& key : keys)
	{
		UpdateTextOnTile(key);
	}
}


