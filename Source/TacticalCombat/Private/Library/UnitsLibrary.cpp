// Fill out your copyright notice in the Description page of Project Settings.


#include "Library/UnitsLibrary.h"
#include "Shared/SharedEnums.h"

FUnitAssetsTableRow UUnitsLibrary::GetDefaultUnitAssetsDataFromUnitType(ETacticalUnitType _unitType)
{
	static UDataTable* pLoadedDataTable = nullptr;
	if (!pLoadedDataTable)
	{
		pLoadedDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Tables/DT_UnitAssets.DT_UnitAssets"));
	}

	if (pLoadedDataTable)
	{
		UEnum* pEnum = StaticEnum<ETacticalUnitType>();
		FString unitTypeName = pEnum->GetNameStringByValue((uint8)_unitType);
		FUnitAssetsTableRow* pRow = pLoadedDataTable->FindRow<FUnitAssetsTableRow>(*unitTypeName, TEXT("Data Table Lookup"));
		if (pRow)
		{
			return *pRow;
		}
	}

	return FUnitAssetsTableRow();
}
