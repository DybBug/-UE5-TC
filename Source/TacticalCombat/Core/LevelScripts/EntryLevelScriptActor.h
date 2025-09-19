// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "EntryLevelScriptActor.generated.h"

/**
 * 
 */
UCLASS()
class TACTICALCOMBAT_API AEntryLevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	FORCEINLINE FName GetLevelName() const {return m_LevelName; }
	
protected:
	UPROPERTY(EditAnywhere, Category = "Property", Meta = (DisplayName = "Level Name"))
	FName m_LevelName;
	
};
