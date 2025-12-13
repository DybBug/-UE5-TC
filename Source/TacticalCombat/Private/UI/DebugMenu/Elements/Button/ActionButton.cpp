// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DebugMenu/Elements/Button/ActionButton.h"
#include "Core/PlayerActions.h"
#include "Action/AbstractAction.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UActionButton::NativeConstruct()
{
	Super::NativeConstruct();

	m_PlayerActions = Cast<APlayerActions>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerActions::StaticClass()));

	const UAbstractAction* pLeftAction = m_PlayerActions->GetLeftClickSelectAction();	
	const UAbstractAction* pRightAction = m_PlayerActions->GetRightClickSelectAction();
	
	bool isValidLeftAction = m_LeftClickActionClass == ((pLeftAction == nullptr) ? nullptr : pLeftAction->GetClass());
	bool isValidRightAction = m_RightClickActionClass == ((pRightAction == nullptr) ? nullptr : pRightAction->GetClass());
	
	if ( isValidLeftAction && isValidRightAction)
	{
		Button->SetBackgroundColor(FColor::Blue);
	}
	else
	{
		Button->SetBackgroundColor(FColor::Silver);
	}
	
	Button->OnClicked.AddDynamic(this, &UActionButton::_OnButtonClicked);
	m_PlayerActions->OnSelectedActionsChanged.AddUObject(this, &UActionButton::OnSelectedActionsChanged);
}

void UActionButton::NativePreConstruct()
{
	Super::NativePreConstruct();
	Text->SetText(FText::FromString(m_ActionText));
}

void UActionButton::OnSelectedActionsChanged(const UAbstractAction* const _leftClickAction, const UAbstractAction* const _rightClickAction)
{
	const UAbstractAction* pLeftAction = m_PlayerActions->GetLeftClickSelectAction();	
	const UAbstractAction* pRightAction = m_PlayerActions->GetRightClickSelectAction();
	
	bool isValidLeftAction = m_LeftClickActionClass == ((pLeftAction == nullptr) ? nullptr : pLeftAction->GetClass());
	bool isValidRightAction = m_RightClickActionClass == ((pRightAction == nullptr) ? nullptr : pRightAction->GetClass());
	
	if ( isValidLeftAction && isValidRightAction)
	{
		Button->SetBackgroundColor(FColor::Blue);
	}
	else
	{
		Button->SetBackgroundColor(FColor::Silver);
	}
}


void UActionButton::_OnButtonClicked()
{
	const UAbstractAction* pLeftAction = m_PlayerActions->GetLeftClickSelectAction();
	const UAbstractAction* pRightAction = m_PlayerActions->GetRightClickSelectAction();
	
	bool isValidLeftAction = m_LeftClickActionClass == ((pLeftAction == nullptr) ? nullptr : pLeftAction->GetClass());
	bool isValidRightAction = m_RightClickActionClass == ((pRightAction == nullptr) ? nullptr : pRightAction->GetClass());
	
	if ( isValidLeftAction && isValidRightAction)
	{
		m_PlayerActions->SetSelectedActionWithNotify(nullptr, nullptr);
	}
	else
	{
		m_PlayerActions->SetSelectedActionWithNotify(m_LeftClickActionClass, m_RightClickActionClass);
	}
}
