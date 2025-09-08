// Fill out your copyright notice in the Description page of Project Settings.


#include "TCGameMode.h"

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
}
