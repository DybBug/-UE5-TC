// Fill out your copyright notice in the Description page of Project Settings.


#include "ModifyTileHeightButton.h"

#include "TacticalCombat/Core/PlayerActions.h"
#include "TacticalCombat/Core/Actions/ModifyTileHeightAction.h"


void UModifyTileHeightButton::OnSelectedActionsChanged(const UAbstractAction* const _leftClickAction, const UAbstractAction* const rightClickAction)
{
	Super::OnSelectedActionsChanged(_leftClickAction, rightClickAction);
	if (UModifyTileHeightAction* pModifyTileHeightAction = Cast<UModifyTileHeightAction>(m_PlayerActions->GetLeftClickSelectAction()))
	{
		pModifyTileHeightAction->SetIsIncrease(true);
	}
	
	if (UModifyTileHeightAction* pModifyTileHeightAction = Cast<UModifyTileHeightAction>(m_PlayerActions->GetRightClickSelectAction()))
    {
    	pModifyTileHeightAction->SetIsIncrease(false);
    }
}
