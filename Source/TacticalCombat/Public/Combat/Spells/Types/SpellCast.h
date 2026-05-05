#pragma once
#include "SpellCast.generated.h"

UENUM(BlueprintType)
enum class ESpellRangePattern: uint8
{
	None			UMETA(DisplayName = "None"),
	Line			UMETA(DisplayName = "Line"),
	Diagonal		UMETA(DisplayName = "Diagonal"),
	HalfDiagonal	UMETA(DisplayName = "HalfDiagonal"),
	Star			UMETA(DisplayName = "Start"),
	Diamond			UMETA(DisplayName = "Diamond"),
	Square			UMETA(DisplayName = "Square"),
	Max				UMETA(Hidden),
};
ENUM_RANGE_BY_COUNT(ESpellRangePattern, ESpellRangePattern::Max);

USTRUCT(BlueprintType)
struct FLineOfSight
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HeightFromGround = 150.0f;  // 시야 계산 시 타일의 지면에서부터의 높이 (예: 유닛의 눈높이)

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float OffsetFromCenter = 0.25f;  // 시야 계산 시 타일 중심에서의 수평 오프셋

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsDrawDebugLine;  // 시야 계산 시 디버그 라인 그리기 여부
};


USTRUCT(BlueprintType)
struct FSpellCast
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESpellRangePattern RangePattern;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntPoint RangeBounds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsRequireLineOfSight;  // 대상에 대한 직선 시야 요구 여부

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLineOfSight LineOfSight;  // 시야 계산에 필요한 추가 정보

};

