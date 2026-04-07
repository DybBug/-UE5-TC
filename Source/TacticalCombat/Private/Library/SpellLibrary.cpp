// Fill out your copyright notice in the Description page of Project Settings.


#include "Library/SpellLibrary.h"

#include "Grid/Grid.h"
#include "Library/GridLibrary.h"

FSpellTableRow USpellLibrary::GetSpellRowDataFromType(ESpellType _type)
{
	static UDataTable* pLoadedDataTable = nullptr;
	if (!pLoadedDataTable)
	{
		pLoadedDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Tables/DT_Spell.DT_Spell"));
	}

	check(pLoadedDataTable);
	if (pLoadedDataTable)
	{
		UEnum* pEnum = StaticEnum<ESpellType>();
		FString unitTypeName = pEnum->GetNameStringByValue((uint8)_type);
		FSpellTableRow* pRow = pLoadedDataTable->FindRow<FSpellTableRow>(*unitTypeName, TEXT("Data Table Lookup"));
		if (pRow)
		{
			return *pRow;
		}
	}

	return FSpellTableRow();
}

TArray<FIntPoint> USpellLibrary::GetIndicesFromPatternAndRange(const FIntPoint& _originIndex, EGridShape _gridShape, ESpellRangePattern _pattern, const FIntPoint& _spellRange)
{
	TArray<FIntPoint> rangeIndices;	
	
	switch (_pattern)
	{
		case ESpellRangePattern::None:
		{
			rangeIndices.Add(_originIndex);
			break;
		}
		case ESpellRangePattern::Line:
		{
			rangeIndices = _GeneratePatternLine(_originIndex, _gridShape, _spellRange);
			_OffsetIndexArray(&rangeIndices, _originIndex);
			break;
		}
		case ESpellRangePattern::Diagonal:
		{
			rangeIndices = _GeneratePatternDiagonal(_originIndex, _gridShape, _spellRange);
			_OffsetIndexArray(&rangeIndices, _originIndex);
			break;
		}
		case ESpellRangePattern::HalfDiagonal:
		{
			rangeIndices = _GeneratePatternHalfDiagonal(_originIndex, _gridShape, _spellRange);
			_OffsetIndexArray(&rangeIndices, _originIndex);
			break;
		}
		case ESpellRangePattern::Star:
		{
			rangeIndices = _GeneratePatternStar(_originIndex, _gridShape, _spellRange);
			_OffsetIndexArray(&rangeIndices, _originIndex);
			break;
		}
		case ESpellRangePattern::Diamond:
		{
			rangeIndices = _GeneratePatternDiamond(_originIndex, _gridShape, _spellRange);
			_OffsetIndexArray(&rangeIndices, _originIndex);
			break;
		}
		case ESpellRangePattern::Square:
		{
			rangeIndices = _GeneratePatternSquare(_originIndex, _gridShape, _spellRange);
			_OffsetIndexArray(&rangeIndices, _originIndex);
			break;
		}
		default:
		{
			checkf(false, TEXT("Invalid EGridShape : %s"), *StaticEnum<EGridShape>()->GetNameStringByValue((uint8)_gridShape));
			break;
		}
	}
	
	return rangeIndices;
}

TArray<FIntPoint> USpellLibrary::_GeneratePatternLine(const FIntPoint& _originIndex, EGridShape _gridShape, const FIntPoint& _range)
{
	TArray<FIntPoint> result;
	for (int32 i = _range.X; i < _range.Y; ++i)
	{
		switch (_gridShape)
		{
			case EGridShape::Square:
			{
				result.AddUnique(FIntPoint(+0, +i));	// Y Right
				result.AddUnique(FIntPoint(+0, -i));	// Y Left
				
				result.AddUnique(FIntPoint(+i, +0));	// X up
				result.AddUnique(FIntPoint(-i, +0));	// X Down
				break;
			}
			case EGridShape::Hexagon:
			{
				result.AddUnique(FIntPoint(+0, +(i * 2)));	// Y Right
				result.AddUnique(FIntPoint(+0, -(i * 2)));	// Y Left
				
				result.AddUnique(FIntPoint(+i, +i));		// X Up, Y Right
				result.AddUnique(FIntPoint(-i, -i));		// X Down, Y Left
				
				result.AddUnique(FIntPoint(+i, -i));		// X Up, Y Left
				result.AddUnique(FIntPoint(-i, +i));		// X Down, Y Right				
				break;
			}
			case EGridShape::Triangle:
			{
				result.AddUnique(FIntPoint(0, i));		// Y Right
				result.AddUnique(FIntPoint(0, -i));	// Y Left
				if (UGridLibrary::IsTriangleTileFacingUp(_originIndex))
				{					
					result.AddUnique(FIntPoint(+FMath::FloorToInt32(i * 0.5f), +FMath::CeilToInt32(i * 0.5f)));
					result.AddUnique(FIntPoint(+FMath::FloorToInt32(i * 0.5f), -FMath::CeilToInt32(i * 0.5f)));
					
					result.AddUnique(FIntPoint(-FMath::FloorToInt32(i * 0.5f), -FMath::CeilToInt32(i * 0.5f)));
					result.AddUnique(FIntPoint(-FMath::FloorToInt32(i * 0.5f), +FMath::CeilToInt32(i * 0.5f)));
				}
				else
				{
					result.AddUnique(FIntPoint(+FMath::CeilToInt32(i * 0.5f), +FMath::FloorToInt32(i * 0.5f)));
					result.AddUnique(FIntPoint(+FMath::CeilToInt32(i * 0.5f), -FMath::FloorToInt32(i * 0.5f)));
					
					result.AddUnique(FIntPoint(-FMath::CeilToInt32(i * 0.5f), -FMath::FloorToInt32(i * 0.5f)));
					result.AddUnique(FIntPoint(-FMath::CeilToInt32(i * 0.5f), +FMath::FloorToInt32(i * 0.5f)));
				}
				break;
			}
			default:
			{
				checkf(false, TEXT("Invalid EGridShape : %s"), *StaticEnum<EGridShape>()->GetNameStringByValue((uint8)_gridShape));
				break;
			}
		}
	}

	return result;
}

TArray<FIntPoint> USpellLibrary::_GeneratePatternDiagonal(const FIntPoint& _originIndex, EGridShape _gridShape, const FIntPoint& _range)
{
	TArray<FIntPoint> result;
	for (int32 i = _range.X; i < _range.Y; ++i)
	{
		switch (_gridShape)
		{
			case EGridShape::Square:
			{
				result.AddUnique(FIntPoint(+i, +i));	// X Up, Y Right
				result.AddUnique(FIntPoint(-i, -i));	// X Down, Y Left
				
				result.AddUnique(FIntPoint(+i, -i));	// X up, Y Left
				result.AddUnique(FIntPoint(-i, +i));	// X Down, Y Right
				break;
			}
			case EGridShape::Hexagon:
			{
				result.AddUnique(FIntPoint(+(i * 2), 0));
				result.AddUnique(FIntPoint(-(i * 2), 0));

				result.AddUnique(FIntPoint(+i, +FMath::CeilToInt32(i * 3.0f)));
				result.AddUnique(FIntPoint(+i, -FMath::CeilToInt32(i * 3.0f)));
				break;
			}
			case EGridShape::Triangle:
			{
				result.AddUnique(FIntPoint(+i, 0));
				result.AddUnique(FIntPoint(-i, 0));
				if (UGridLibrary::IsTriangleTileFacingUp(_originIndex))
				{
					result.AddUnique(FIntPoint(+FMath::FloorToInt32(i * 0.5f), +FMath::FloorToInt32(i * 3.0f)));
					result.AddUnique(FIntPoint(+FMath::FloorToInt32(i * 0.5f), -FMath::FloorToInt32(i * 3.0f)));

					result.AddUnique(FIntPoint(-FMath::FloorToInt32(i * 0.5f), +FMath::FloorToInt32(i * 3.0f)));
					result.AddUnique(FIntPoint(-FMath::FloorToInt32(i * 0.5f), -FMath::FloorToInt32(i * 3.0f)));
				}
				else
				{
					result.AddUnique(FIntPoint(+FMath::CeilToInt32(i * 0.5f), +FMath::CeilToInt32(i * 3.0f)));
					result.AddUnique(FIntPoint(+FMath::CeilToInt32(i * 0.5f), -FMath::CeilToInt32(i * 3.0f)));

					result.AddUnique(FIntPoint(-FMath::CeilToInt32(i * 0.5f), +FMath::CeilToInt32(i * 3.0f)));
					result.AddUnique(FIntPoint(-FMath::CeilToInt32(i * 0.5f), -FMath::CeilToInt32(i * 3.0f)));
				}
				break;
			}
			default:
			{
				checkf(false, TEXT("Invalid EGridShape : %s"), *StaticEnum<EGridShape>()->GetNameStringByValue((uint8)_gridShape));
				break;
			}
		}
		
	}
	return result;
}

TArray<FIntPoint> USpellLibrary::_GeneratePatternHalfDiagonal(const FIntPoint& _originIndex, EGridShape _gridShape, const FIntPoint& _range)
{
	int32 x = _range.X * 0.5f;
	int32 y = _range.Y * 0.5f;
	return _GeneratePatternDiagonal(_originIndex, _gridShape, FIntPoint(x, y));
}

TArray<FIntPoint> USpellLibrary::_GeneratePatternStar(const FIntPoint& _originIndex, EGridShape _gridShape, const FIntPoint& _range)
{
	TArray<FIntPoint> result = _GeneratePatternLine(_originIndex, _gridShape, _range);
	if (_gridShape == EGridShape::Square)
	{
		TArray<FIntPoint> diagonalIndices = _GeneratePatternDiagonal(_originIndex, _gridShape, _range);
		for (const FIntPoint& diagonalIndex : diagonalIndices)
		{
			result.AddUnique(diagonalIndex);
		}
	}
	else
	{
		TArray<FIntPoint> halfDiagonalIndices = _GeneratePatternHalfDiagonal(_originIndex, _gridShape, _range);
		for (const FIntPoint& halfDiagonalIndex : halfDiagonalIndices)
		{
			result.AddUnique(halfDiagonalIndex);
		}
	}
	return result;
}

TArray<FIntPoint> USpellLibrary::_GeneratePatternDiamond(const FIntPoint& _originIndex, EGridShape _gridShape, const FIntPoint& _range)
{
	TArray<FIntPoint> result;
	for (int32 i = _range.X; i < _range.Y; ++i)
	{
		switch (_gridShape)
		{
			case EGridShape::Square:
			{
				for (int j = 0; j <= i; ++j)
				{
					result.AddUnique(FIntPoint(j, -(i - j)));
					result.AddUnique(FIntPoint((i - j),j));

					result.AddUnique(FIntPoint(-j, (i - j)));
					result.AddUnique(FIntPoint(-(i - j), -j));
				}
				break;
			}
			case EGridShape::Hexagon:
			{
				for (int j = 0; j <= i; ++j)
				{					
					result.AddUnique(FIntPoint(+j, -((i * 2) - j)));
					result.AddUnique(FIntPoint(-j, +((i * 2) - j)));

					result.AddUnique(FIntPoint(+(i - j), +((i * 2) - (i - j))));
					result.AddUnique(FIntPoint(-(i - j), -((i * 2) - (i - j))));
				}

				for (int j = -(i - 2); j <= i - 2; ++j)
				{
					result.AddUnique(FIntPoint(+i ,+j));
					result.AddUnique(FIntPoint(-i, -j));
				}
				break;
			}
			case EGridShape::Triangle:
			{
				bool isFacingUp = UGridLibrary::IsTriangleTileFacingUp(_originIndex);
				for (int j = 0; j <= i; ++j)
				{
					{
						int32 x = isFacingUp ? j : -j;
						int32 y = (i * 2) - j;
						result.AddUnique(FIntPoint(+x, -y));
						result.AddUnique(FIntPoint(-x, +y));

						if (j != i) result.AddUnique(FIntPoint(+x, -y + 1));
						if (j != 0) result.AddUnique(FIntPoint(-x, y + 1));
					}
					{
						int32 x = isFacingUp ? (i - j) : -(i - j);
						int32 y = (i * 2) - (i - j);
						result.AddUnique(FIntPoint(-x, -y));
						result.AddUnique(FIntPoint(+x, +y));

						if (j != i) result.AddUnique(FIntPoint(-x, -y - 1));
						if (j != 0) result.AddUnique(FIntPoint(+x, y - 1));
					}					
				}

				for (int32 j = -i; j <= i; ++j)
				{
					{
						int32 x = isFacingUp ? i : -i;
						int32 y = j;
						result.AddUnique(FIntPoint(x, y));
					}
					{
						int32 x = isFacingUp ? -i : i;
						int32 y = -j;
						result.AddUnique(FIntPoint(x, y));
					}
				}
				break;
			}
			default:
			{
				checkf(false, TEXT("Invalid EGridShape : %s"), *StaticEnum<EGridShape>()->GetNameStringByValue((uint8)_gridShape));
				break;
			}
		}
	}
	return result;
}

TArray<FIntPoint> USpellLibrary::_GeneratePatternSquare(const FIntPoint& _originIndex, EGridShape _gridShape, const FIntPoint& _range)
{
	TArray<FIntPoint> result;
	for (int32 i = _range.X; i < _range.Y; ++i)
	{
		switch (_gridShape)
		{
			case EGridShape::Square:
			{
				for (int32 j = -i; j <= i; ++j)
				{
					result.AddUnique(FIntPoint(+j, -i));
					result.AddUnique(FIntPoint(+i, +j));
					
					result.AddUnique(FIntPoint(-j, +i));
					result.AddUnique(FIntPoint(-i, -j));
				}
				break;
			}
			case EGridShape::Hexagon:
			case EGridShape::Triangle:
			{
				for (int32 j = -i; j <= i; ++j)
				{
					result.AddUnique(FIntPoint(j, -i * 2));
					result.AddUnique(FIntPoint(-j, i * 2));

					if (i != j)
					{
						result.AddUnique(FIntPoint(j, (-i * 2) + 1));
						result.AddUnique(FIntPoint(-j, (i * 2) - 1));	
					}
				}

				for (int32 j = -i * 2; j <= i * 2; ++j)
				{
					result.AddUnique(FIntPoint(+i, +j));
					result.AddUnique(FIntPoint(-i, -j));
				}
				break;
			}
			default:
			{
				checkf(false, TEXT("Invalid EGridShape : %s"), *StaticEnum<EGridShape>()->GetNameStringByValue((uint8)_gridShape));
				break;
			}
		}
	}
	return result;
}


void USpellLibrary::_OffsetIndexArray(TArray<FIntPoint>* const _indices, const FIntPoint& _originIndex)
{
	for (int i = 0; i < _indices->Num(); ++i)
	{
		(*_indices)[i] = (*_indices)[i] + _originIndex;
	}
}
