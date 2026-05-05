// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Dispatcher/LevelLoadingDispatcher.h"
#include "LevelLoadingSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class TACTICALCOMBAT_API ULevelLoadingSubsystem : public UGameInstanceSubsystem, public LevelLoadingDispatcher
{
	GENERATED_BODY()

public:
	void LoadLevel(const FName& _levelName, bool _bIsForce = false);

private:
	FName m_LoadedLevelName;

	FTimerHandle m_LevelLoadedTimerHandle;

private:
	UFUNCTION()
	void _OnLevelLoaded();
	
};
