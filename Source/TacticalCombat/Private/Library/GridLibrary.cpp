// Fill out your copyright notice in the Description page of Project Settings.


#include "Library/GridLibrary.h"

#include "Shared/SharedEnums.h"
#include "Grid/Types/GridShapeData.h"

FGridShapeData UGridLibrary::GetGridShape(EGridShape _gridShape)
{
	static UDataTable* pLoadedDataTable;
	if (pLoadedDataTable == nullptr)
	{
		pLoadedDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Grids/GridShapes/DT_GridShape.DT_GridShape"));
	}
	
	// 성공적으로 로드됨
	if (pLoadedDataTable)
	{
		FName girdShapeName = *(StaticEnum<EGridShape>()->GetNameStringByValue((uint8)_gridShape));
		FGridShapeData* pGridShapeData = pLoadedDataTable->FindRow<FGridShapeData>(girdShapeName, TEXT("Data Table Lookup"));
		if (pGridShapeData != nullptr)
		{
			return *pGridShapeData;
		}		
	}
	return FGridShapeData();
}
