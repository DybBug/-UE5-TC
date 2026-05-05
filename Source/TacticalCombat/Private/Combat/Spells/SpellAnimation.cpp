// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Spells/SpellAnimation.h"

#include "Combat/Spells/Components/AnimatedShape.h"
#include "Grid/Grid.h"


// Sets default values
ASpellAnimation::ASpellAnimation()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void ASpellAnimation::BeginPlay()
{
	Super::BeginPlay();
}


void ASpellAnimation::PostSpawnInitialize(AGrid* const _pGrid, const FIntPoint& _originIndex, const TArray<FIntPoint>& _targetIndices)
{
	m_Grid = TWeakObjectPtr(_pGrid);;
	m_OriginIndex = _originIndex;
	m_TargetIndices = _targetIndices;
	PlaySpellAnimation();
}

void ASpellAnimation::PlaySpellAnimation()
{
	AAnimatedShape* pSpawnedAnimatedShape = GetWorld()->SpawnActor<AAnimatedShape>(AAnimatedShape::StaticClass());
	FGridShapeTableRow gridRowData = m_Grid->GetGridShapeData();
	UStaticMesh* const pStaticMesh = gridRowData.Mesh;
	UMaterialInstance* const pMaterial = gridRowData.MeshMaterial;
	const FLinearColor& color = FLinearColor::Red;
	FTransform originTransform = _GetTileTransform(m_OriginIndex);
	originTransform.SetScale3D(FVector(1.0f, 1.0f, 1.0f));
	
	for (uint8 i = 0; i < m_TargetIndices.Num(); ++i)
	{		
		const FTransform& targetTransform = _GetTileTransform(m_TargetIndices[i]);
		
			pSpawnedAnimatedShape->PostSpawnInitialize({
				.pMesh = pStaticMesh,
				.pMaterial = pMaterial,
				.Color = color,
				.OriginTransform = originTransform,
				.TargetTransform = targetTransform,
				.PlayRate = 1.0f
			});
		// NOTE: 이벤트를 여러번 받을 필요 없이 한번만 받음
		if (i == 0)
		{
			pSpawnedAnimatedShape->BindAnimatedShapeFinished(this, &ASpellAnimation::HandleAnimatedShapeFinished);
		}
	}
}

FTransform ASpellAnimation::_GetTileTransform(const FIntPoint& _index)
{
	return m_Grid->GetGridTileMap().Find(_index)->Transform;
}

void ASpellAnimation::HandleAnimatedShapeFinished()
{
	BroadcastSpellAnimationFinished(this);
	Destroy();
}

