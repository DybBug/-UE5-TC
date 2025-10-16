// Fill out your copyright notice in the Description page of Project Settings.


#include "AbstractAction.h"
#include "TacticalCombat/Core/PlayerActions.h"

// Sets default values
UAbstractAction::UAbstractAction()
{

}

void UAbstractAction::Initialize(APlayerActions* const _playerActions)
{
	m_PlayerActions = _playerActions;
}
