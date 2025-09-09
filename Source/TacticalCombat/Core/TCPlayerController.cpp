// Fill out your copyright notice in the Description page of Project Settings.


#include "TCPlayerController.h"

#include "Camera/CameraComponent.h"

void ATCPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetShowMouseCursor(true);
}


