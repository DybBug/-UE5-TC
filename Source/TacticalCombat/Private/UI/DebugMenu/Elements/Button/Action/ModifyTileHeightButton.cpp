// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DebugMenu/Elements/Button/Action/ModifyTileHeightButton.h"

#include "Core/PlayerActions.h"
#include "Action/Grid/ModifyTileHeightAction.h"


void UModifyTileHeightButton::OnSelectedActionsChanged(const UAbstractAction* const _leftClickAction, const UAbstractAction* const rightClickAction)
{
	Super::OnSelectedActionsChanged(_leftClickAction, rightClickAction);
	if (IsCurrentSelectedAction())
	{
		if (UModifyTileHeightAction* pModifyTileHeightAction = Cast<UModifyTileHeightAction>(m_PlayerActions->GetLeftClickSelectAction()))
		{
			pModifyTileHeightAction->SetIsIncrease(true);
		}
		
		if (UModifyTileHeightAction* pModifyTileHeightAction = Cast<UModifyTileHeightAction>(m_PlayerActions->GetRightClickSelectAction()))
	    {
    		pModifyTileHeightAction->SetIsIncrease(false);
	    }	
	}
}
