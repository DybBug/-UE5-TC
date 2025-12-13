// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/TCGameMode.h"
#include "GameFramework/HUD.h"

ATCGameMode::ATCGameMode()
{
	static ConstructorHelpers::FClassFinder<APlayerController> playerControllerClassFinder(TEXT("/Script/TacticalCombat.TCPlayerController"));
	if (playerControllerClassFinder.Succeeded())
	{
		PlayerControllerClass = playerControllerClassFinder.Class;
	}

	static ConstructorHelpers::FClassFinder<APawn> defaultPawnClassFinder(TEXT("/Script/TacticalCombat.TCPawn"));
	if (defaultPawnClassFinder.Succeeded())
	{
		DefaultPawnClass = defaultPawnClassFinder.Class;
	}

	static ConstructorHelpers::FClassFinder<AHUD> hudClassFinder(TEXT("/Script/TacticalCombat.TCHUD"));
	if (hudClassFinder.Succeeded())
	{
		HUDClass = hudClassFinder.Class;
	}
}
