// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid/GridInstancedStaticMeshComponent.h"
#include "Shared/SharedEnums.h"

struct FTileStateInfo
{
	FColor Color;
	int32 Priority;  // 낮을수록 높은 우선순위
};

static TMap<ETileStateFlags, FTileStateInfo> s_TileStateFlagsToInfo =
{
	{ETileStateFlags::None,		{FColor(0, 0, 0, 0), 999 }},
	{ETileStateFlags::Selected,	{FColor(255,0,0), 1}},
	{ETileStateFlags::Hovered,	{FColor(255,255,0), 2}},
	{ETileStateFlags::Neighbor,	{FColor(169, 7, 228), 3}},
	{ETileStateFlags::InPath,		{FColor(0,0,255), 4}},
	{ETileStateFlags::Discovered, {FColor(179, 27, 77), 5}},
	{ETileStateFlags::Analyzed,	{FColor(248, 112, 158), 6}}	
};

static TMap<ETileType, FColor> s_TileTypeToColor =
{
	{ETileType::None, FColor(0, 0, 0)},
	{ETileType::Normal, FColor(255, 255, 255)},
	{ETileType::Obstacle, FColor(255, 50, 50)},
	{ETileType::DoubleCost, FColor(120, 120, 120)},
	{ETileType::TripleCost, FColor(40, 40, 40)},
	{ETileType::FlyingUnitsOnly, FColor(0, 177, 228)}
};

UGridInstancedStaticMeshComponent::UGridInstancedStaticMeshComponent()
{
	
}

void UGridInstancedStaticMeshComponent::InitializeGridMeshInst(
	UStaticMesh* const _pMesh,
	UMaterialInstance* const _pMaterial,
	bool _IsColorBasedOnTileType,
	ECollisionEnabled::Type _collisionEnabled)
{
	SetStaticMesh(_pMesh);
	SetMaterial(0, _pMaterial);
	m_bIsColorBasedOnTileType = _IsColorBasedOnTileType;
	SetCollisionEnabled(_collisionEnabled);
}

void UGridInstancedStaticMeshComponent::AddInstance(const FIntPoint& _index, const FTransform& _transform, uint8 _tileStateMask, ETileType _tileType)
{
	RemoveInstance(_index);
	UInstancedStaticMeshComponent::AddInstance(_transform, false);
	m_InstanceIndices.Add(_index);
	
	FColor tileColor = m_bIsColorBasedOnTileType ? _GetColorFromType(_tileType) : _GetColorFromState(_tileStateMask);
	int32 index = m_InstanceIndices.Num() - 1;
	SetCustomDataValue(index, 0, tileColor.R / 255.f);
	SetCustomDataValue(index, 1, tileColor.G / 255.f);
	SetCustomDataValue(index, 2, tileColor.B / 255.f);
	SetCustomDataValue(index, 3, tileColor.A / 255.f);
}

void UGridInstancedStaticMeshComponent::RemoveInstance(const FIntPoint& _index)
{
	if (m_InstanceIndices.Contains(_index))
	{
		int32 foundInstanceIndex = m_InstanceIndices.Find(_index);
		UInstancedStaticMeshComponent::RemoveInstance(foundInstanceIndex);
		m_InstanceIndices.Remove(_index);
	}
}

void UGridInstancedStaticMeshComponent::ClearInstances()
{
	UInstancedStaticMeshComponent::ClearInstances();
    m_InstanceIndices.Empty();
}

FColor UGridInstancedStaticMeshComponent::_GetColorFromState(uint8 _tileStateMask)
{
	ETileStateFlags bestFlag = ETileStateFlags::None;
	int32 bestPriority = INT_MAX;
    
	for (const auto& [flag, info] : s_TileStateFlagsToInfo)
	{
		if (flag != ETileStateFlags::None && 
			(_tileStateMask & static_cast<uint8>(flag)) != 0)
		{
			if (info.Priority < bestPriority)
			{
				bestPriority = info.Priority;
				bestFlag = flag;
			}
		}
	}
    
	return s_TileStateFlagsToInfo[bestFlag].Color;
}

FColor UGridInstancedStaticMeshComponent::_GetColorFromType(ETileType _tileType)
{
	return s_TileTypeToColor[_tileType];
}
