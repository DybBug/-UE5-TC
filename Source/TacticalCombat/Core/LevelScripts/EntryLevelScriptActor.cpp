// Fill out your copyright notice in the Description page of Project Settings.


#include "EntryLevelScriptActor.h"

#include "TacticalCombat/Subsystems/LevelLoadingSubsystem.h"

void AEntryLevelScriptActor::BeginPlay()
{
	Super::BeginPlay();

	ULevelLoadingSubsystem* pLevelLoader = GetWorld()->GetGameInstance()->GetSubsystem<ULevelLoadingSubsystem>();
	pLevelLoader->LoadLevel(m_LevelName);
}
