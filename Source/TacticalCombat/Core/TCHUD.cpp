// Fill out your copyright notice in the Description page of Project Settings.


#include "TCHUD.h"

#include "Blueprint/UserWidget.h"
#include "../UI/DebugMenu.h"

ATCHUD::ATCHUD()
{
	static ConstructorHelpers::FClassFinder<UDebugMenu> debugMenuClassFinder(TEXT("/Game/Widgets/W_DebugMenu"));
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
