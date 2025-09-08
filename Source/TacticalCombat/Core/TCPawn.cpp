// Fill out your copyright notice in the Description page of Project Settings.


#include "TCPawn.h"

// Sets default values
ATCPawn::ATCPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATCPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATCPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATCPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

