// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PGChattingWidget.generated.h"

class UEditableTextBox;
class UButton;
class UTextBlock;
/**
 * 
 */
UCLASS()
class PLAYGROUND_API UPGChattingWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void AppendChatMessage(const FString& Message);

protected:
	virtual void NativeOnInitialized() override;

private:
	UFUNCTION()
	void HandleSendClicked();

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> EditableTextBox_Message;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Send;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_ChatHistory;

	FString ChatHistory;
};
