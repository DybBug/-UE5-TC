// Fill out your copyright notice in the Description page of Project Settings.


#include "Action/AbstractAction.h"
#include "Core/PlayerActions.h"

// Sets default values
UAbstractAction::UAbstractAction()
{

}

void UAbstractAction::BeginDestroy()
{
	Super::BeginDestroy();
	Execute(FIntPoint(-999, -999));
}

void UAbstractAction::Initialize(APlayerActions* const _pPlayerActions)
{
	m_PlayerActions = _pPlayerActions;
}
