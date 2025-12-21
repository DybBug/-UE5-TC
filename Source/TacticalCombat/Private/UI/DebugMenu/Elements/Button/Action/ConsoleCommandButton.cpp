// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DebugMenu/Elements/Button/Action/ConsoleCommandButton.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetSystemLibrary.h"


void UConsoleCommandButton::NativeConstruct()
{
	Super::NativeConstruct();
	Button->OnClicked.AddDynamic(this, &UConsoleCommandButton::_OnClickedButton);
}

void UConsoleCommandButton::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	Text->SetText(FText::FromString(m_ConsoleCommand));
}

void UConsoleCommandButton::_OnClickedButton()
{
	UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), m_ConsoleCommand);
}
