// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerActions.h"

#include "Grid/Grid.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APlayerActions::APlayerActions()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayerActions::BeginPlay()
{
	Super::BeginPlay();

	m_Grid = Cast<AGrid>(UGameplayStatics::GetActorOfClass(GetWorld(), AGrid::StaticClass())) ;	
	
}

// Called every frame
void APlayerActions::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	_UpdateTileUnderCursor();
}

void APlayerActions::_UpdateTileUnderCursor()
{
	const FIntPoint hoveredTileIndex =  m_Grid->GetTileIndexUnderCursor(0);
	if (m_HoveredTileIndex != hoveredTileIndex)
	{
		m_Grid->RemoveStateToTile(m_HoveredTileIndex, ETileStateFlags::Hovered);
		
		m_HoveredTileIndex = hoveredTileIndex;
		m_Grid->AddStateToTile(m_HoveredTileIndex, ETileStateFlags::Hovered);
	}
}


