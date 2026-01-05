// Fill out your copyright notice in the Description page of Project Settings.


#include "Library/UnitsLibrary.h"
#include "Shared/SharedEnums.h"

FUnitTableRow UUnitsLibrary::GetDefaultUnitAssetsDataFromUnitType(ETacticalUnitType _unitType)
{
	static UDataTable* pLoadedDataTable = nullptr;
	if (!pLoadedDataTable)
	{
		pLoadedDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Tables/DT_Units.DT_Units"));
	}

	check(pLoadedDataTable);
	if (pLoadedDataTable)
	{
		UEnum* pEnum = StaticEnum<ETacticalUnitType>();
		FString unitTypeName = pEnum->GetNameStringByValue((uint8)_unitType);
		FUnitTableRow* pRow = pLoadedDataTable->FindRow<FUnitTableRow>(*unitTypeName, TEXT("Data Table Lookup"));
		if (pRow)
		{
			return *pRow;
		}
	}

	return FUnitTableRow();
}
