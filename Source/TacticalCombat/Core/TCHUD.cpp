// Fill out your copyright notice in the Description page of Project Settings.


#include "TCHUD.h"

#include "Blueprint/UserWidget.h"
#include "../UI/DebugMenu/DebugMenu.h"
#include "Grid/Other/DebugTextOnTiles.h"

ATCHUD::ATCHUD()
{
	static ConstructorHelpers::FClassFinder<UDebugMenu> debugMenuClassFinder(TEXT("/Game/Widgets/DebugMenu/W_DebugMenu"));
	if (debugMenuClassFinder.Succeeded())
	{
		m_DebugMenuClass = debugMenuClassFinder.Class;
	}
}

void ATCHUD::BeginPlay()
{
	Super::BeginPlay();

	m_DebugMenu = CreateWidget<UDebugMenu>(GetWorld(), m_DebugMenuClass);
	m_DebugMenu->AddToViewport();
}
