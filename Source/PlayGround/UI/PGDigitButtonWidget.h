// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PGDigitButtonWidget.generated.h"

class UButton;
class UTextBlock;
/**
 * 
 */

DECLARE_MULTICAST_DELEGATE_OneParam(
	FOnDigitClicked,
	int32
);

UCLASS()
class PLAYGROUND_API UPGDigitButtonWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	virtual void NativePreConstruct() override;

	UFUNCTION()
	void HandleDigitClicked();

	FOnDigitClicked OnDigitClicked;

private:
	UPROPERTY(EditAnywhere, Category = "Keypad", meta = (ClampMin= "1", ClampMax = "9"))
	int32 Digit = 1;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Digit;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Digit;

};
