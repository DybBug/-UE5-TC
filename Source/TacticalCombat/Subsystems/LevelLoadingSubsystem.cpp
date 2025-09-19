// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelLoadingSubsystem.h"
#include "Kismet/GameplayStatics.h"

void ULevelLoadingSubsystem::LoadLevel(const FName& _levelName, bool _bIsForce)
{
	if (!_bIsForce && m_LoadedLevelName == _levelName)
		return;
	
	ULevelStreaming* pPrevLevel = UGameplayStatics::GetStreamingLevel(GetWorld(), m_LoadedLevelName);
	if (pPrevLevel)
	{
		pPrevLevel->SetShouldBeLoaded(false);
		pPrevLevel->SetShouldBeVisible(false);
	}

	m_LoadedLevelName = _levelName;
	ULevelStreaming* pLoadedLevel = UGameplayStatics::GetStreamingLevel(GetWorld(), m_LoadedLevelName);
	if (!pLoadedLevel)
	{
		UKismetSystemLibrary::PrintWarning(FString::Printf(TEXT("Error - Load Level Failed - invalid level name: %s"), *m_LoadedLevelName.ToString()));
		return;
	}
	pLoadedLevel->SetShouldBeLoaded(true);
	pLoadedLevel->SetShouldBeVisible(true);
}
