// Fill out your copyright notice in the Description page of Project Settings.


#include "GridInstancedStaticMeshComponent.h"
#include "TacticalCombat/Misc/Enums.h"

UGridInstancedStaticMeshComponent::UGridInstancedStaticMeshComponent()
{
	
}

void UGridInstancedStaticMeshComponent::AddInstance(const FIntPoint& _index, const FTransform& _transform, uint8 _tileStateMask)
{
	RemoveInstance(_index);
	UInstancedStaticMeshComponent::AddInstance(_transform, false);
	m_InstanceIndices.Add(_index);
	
	FColor tileColor = _GetColorFromState(_tileStateMask);
	int32 index = m_InstanceIndices.Num() - 1;
	SetCustomDataValue(index, 0, tileColor.R);
	SetCustomDataValue(index, 1, tileColor.G);
	SetCustomDataValue(index, 2, tileColor.B);
	SetCustomDataValue(index, 3, tileColor.A);
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
	bool isSelected = (_tileStateMask & static_cast<uint8>(ETileStateFlags::Selected)) != 0;
	if (isSelected)
	{
		return FColor(255,0,0);
	}

	bool isHovered = (_tileStateMask & static_cast<uint8>(ETileStateFlags::Hovered)) != 0;
	if (isHovered)
	{
		return FColor(255,255,0);
	}

	bool isNeighbor = (_tileStateMask & static_cast<uint8>(ETileStateFlags::Neighbor)) != 0;
	if (isNeighbor)
	{
		return FColor(169, 7, 228);
	}

	bool isInPath = (_tileStateMask & static_cast<uint8>(ETileStateFlags::InPath)) != 0;
	if (isInPath)
	{
		return FColor(0,0,255);
	}

	bool isDiscovered = (_tileStateMask & static_cast<uint8>(ETileStateFlags::Discovered)) != 0;
	if (isDiscovered)
	{
		return FColor(179, 27, 77);
	}

	bool isAnalyzed = (_tileStateMask & static_cast<uint8>(ETileStateFlags::Analyzed)) != 0;
	if (isAnalyzed)
	{
		return FColor(248, 112, 158);
	}
	
	FColor color = FColor::Black;
	color.A = 0.0f;
	return color;
}
