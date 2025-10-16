// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionButton.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "TacticalCombat/Core/PlayerActions.h"
#include "TacticalCombat/Core/Actions/AbstractAction.h"

void UActionButton::NativeConstruct()
{
	Super::NativeConstruct();

	m_PlayerActions = Cast<APlayerActions>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerActions::StaticClass()));

	const UAbstractAction* pAction = m_PlayerActions->GetLeftClickSelectAction();
	if (pAction == nullptr)
	{
		Button->SetBackgroundColor(FColor::Silver);
	}
	else
	{
		if (m_PlayerActions->GetLeftClickSelectAction()->GetClass() == m_LeftClickActionClass.Get()->GetClass())
		{
			Button->SetBackgroundColor(FColor::Blue);
		}
		else
		{
			Button->SetBackgroundColor(FColor::Silver);
		}
	}
	
	Button->OnClicked.AddDynamic(this, &UActionButton::_OnButtonClicked);
	m_PlayerActions->OnSelectedActionsChanged.AddUObject(this, &UActionButton::_OnSelectedActionsChanged);
}

void UActionButton::NativePreConstruct()
{
	Super::NativePreConstruct();
	Text->SetText(FText::FromString(m_ActionText));
}

void UActionButton::_OnButtonClicked()
{
	const UAbstractAction* pLeftClickSelectedAction = m_PlayerActions->GetLeftClickSelectAction();
	if (pLeftClickSelectedAction == nullptr || m_PlayerActions->GetLeftClickSelectAction()->GetClass() != m_LeftClickActionClass.Get())
	{
		m_PlayerActions->SetSelectedActionWithNotify(m_LeftClickActionClass, m_RightClickActionClass);
	}
	else
	{
		m_PlayerActions->SetSelectedActionWithNotify(nullptr, nullptr);
	}
}

void UActionButton::_OnSelectedActionsChanged(const UAbstractAction* const _leftClickAction, const UAbstractAction* const _rightClickAction)
{
	if (m_PlayerActions->GetLeftClickSelectAction() == nullptr)
	{
		Button->SetBackgroundColor(FColor::Silver);
	}
	else if (m_PlayerActions->GetLeftClickSelectAction()->GetClass() == m_LeftClickActionClass.Get())
	{
		Button->SetBackgroundColor(FColor::Blue);
	}
}
