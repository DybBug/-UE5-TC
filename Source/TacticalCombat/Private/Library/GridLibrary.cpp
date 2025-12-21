// Fill out your copyright notice in the Description page of Project Settings.


#include "Library/GridLibrary.h"

#include "Shared/SharedEnums.h"
#include "Table/Rows/GridShapeTableRow.h"

FGridShapeTableRow UGridLibrary::GetGridShape(EGridShape _gridShape)
{
	static UDataTable* pLoadedDataTable = nullptr;;
	if (pLoadedDataTable == nullptr)
	{
		pLoadedDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Tables/DT_GridShape.DT_GridShape"));
	}
	
	// 성공적으로 로드됨
	if (pLoadedDataTable)
	{
		FName girdShapeName = *(StaticEnum<EGridShape>()->GetNameStringByValue((uint8)_gridShape));
		FGridShapeTableRow* pGridShapeData = pLoadedDataTable->FindRow<FGridShapeTableRow>(girdShapeName, TEXT("Data Table Lookup"));
		if (pGridShapeData != nullptr)
		{
			return *pGridShapeData;
		}		
	}
	return FGridShapeTableRow();
}
