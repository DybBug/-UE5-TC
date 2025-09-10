// Fill out your copyright notice in the Description page of Project Settings.


#include "ConsoleCommandEditableTextBox.h"
#include "Kismet/KismetSystemLibrary.h"

TSharedRef<SWidget> UConsoleCommandEditableTextBox::RebuildWidget()
{
	TSharedRef<SWidget> pWidget = Super::RebuildWidget();

	OnTextCommitted.AddDynamic(this, &UConsoleCommandEditableTextBox::_OnTextCommitted);
	SetHintText(FText::FromString("Console Command..."));
	return pWidget;
}

void UConsoleCommandEditableTextBox::_OnTextCommitted(const FText& _text, ETextCommit::Type _commitMethod)
{
	if (_commitMethod == ETextCommit::OnEnter)
	{
		UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), _text.ToString());
		SetFocus(); // 다시 포커스
	}
}

