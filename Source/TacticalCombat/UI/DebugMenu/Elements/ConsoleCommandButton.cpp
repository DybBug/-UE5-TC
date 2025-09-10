// Fill out your copyright notice in the Description page of Project Settings.


#include "ConsoleCommandButton.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetSystemLibrary.h"


bool UConsoleCommandButton::Initialize()
{
	bool result = Super::Initialize();

	Button->OnClicked.AddDynamic(this, &UConsoleCommandButton::_OnClickedButton);
	
	return result;
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
