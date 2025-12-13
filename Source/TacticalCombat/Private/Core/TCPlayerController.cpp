// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/TCPlayerController.h"

#include "Camera/CameraComponent.h"

void ATCPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetShowMouseCursor(true);
}


