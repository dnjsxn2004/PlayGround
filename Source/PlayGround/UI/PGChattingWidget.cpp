// Fill out your copyright notice in the Description page of Project Settings.


#include "PGChattingWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "../PlayerController/PGPlayerController.h"

void UPGChattingWidget::AppendChatMessage(const FString& Message)
{
	if (!IsValid(TextBlock_ChatHistory))
	{
		return;
	}

	if (!ChatHistory.IsEmpty())
	{
		ChatHistory += TEXT("\n");
	}

	ChatHistory += Message;

	TextBlock_ChatHistory->SetText(FText::FromString(ChatHistory));
}

void UPGChattingWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (!IsValid(Button_Send))
	{
		return;
	}

	Button_Send->OnClicked.AddDynamic(
		this,
		&UPGChattingWidget::HandleSendClicked
	);
}

void UPGChattingWidget::HandleSendClicked()
{
	if (!IsValid(EditableTextBox_Message))
	{
		return;
	}

	FString Message = EditableTextBox_Message->GetText().ToString();
	Message.TrimStartAndEndInline();

	if (Message.IsEmpty())
	{
		return;
	}

	APGPlayerController* PGPlayerContoller = GetOwningPlayer<APGPlayerController>();

	if (!IsValid(PGPlayerContoller))
	{
		return;
	}

	PGPlayerContoller->ServerRPCSendChatMessage(Message);

	EditableTextBox_Message->SetText(FText::GetEmpty());
}
