// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PGNumberInputWidget.generated.h"

class UUniformGridPanel;
class UTextBlock;
class UButton;
/**
 * 
 */
UCLASS()
class PLAYGROUND_API UPGNumberInputWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void AppendDigit(int32 Digit);

	UFUNCTION()
	void ShowInputError(const FText& Message);

	UFUNCTION()
	void ClearInput();

protected:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void HandleDeleteClicked();

	UFUNCTION()
	void HandleSubmitClicked();

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> UniformGridPanel_Digits;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Input;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Delete;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Submit;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Error;

	FString CurrentInput;
	
};
