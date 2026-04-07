// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Table/Rows/SpellTableRow.h"
#include "SpellLibrary.generated.h"

enum class EGridShape : uint8;
enum class ESpellType: uint8;
/**
 * 
 */
UCLASS()
class TACTICALCOMBAT_API USpellLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Spell Library")
	static FSpellTableRow GetSpellRowDataFromType(ESpellType _type);

	UFUNCTION(BlueprintPure, Category = "Spell Library")
	static TArray<FIntPoint> GetIndicesFromPatternAndRange(const FIntPoint& _originIndex, EGridShape _gridShape, ESpellRangePattern _pattern, const FIntPoint& _spellRange);

private:
	static TArray<FIntPoint> _GeneratePatternLine(const FIntPoint& _originIndex, EGridShape _gridShape, const FIntPoint& _range);
	static TArray<FIntPoint> _GeneratePatternDiagonal(const FIntPoint& _originIndex, EGridShape _gridShape, const FIntPoint& _range);
	static TArray<FIntPoint> _GeneratePatternHalfDiagonal(const FIntPoint& _originIndex, EGridShape _gridShape, const FIntPoint& _range);
	static TArray<FIntPoint> _GeneratePatternStar(const FIntPoint& _originIndex, EGridShape _gridShape, const FIntPoint& _range);
	static TArray<FIntPoint> _GeneratePatternDiamond(const FIntPoint& _originIndex, EGridShape _gridShape, const FIntPoint& _range);
	static TArray<FIntPoint> _GeneratePatternSquare(const FIntPoint& _originIndex, EGridShape _gridShape, const FIntPoint& _range);
	
	static void _OffsetIndexArray(TArray<FIntPoint>* const _indices, const FIntPoint& _originIndex);
};
