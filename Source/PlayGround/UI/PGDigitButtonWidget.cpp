// Fill out your copyright notice in the Description page of Project Settings.

#include "PGDigitButtonWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UPGDigitButtonWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (!IsValid(Button_Digit))
	{
		return;
	}

	Button_Digit->OnClicked.AddDynamic(
		this,
		&UPGDigitButtonWidget::HandleDigitClicked
	);
}

void UPGDigitButtonWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (!IsValid(TextBlock_Digit))
	{
		return;
	}

	TextBlock_Digit->SetText(FText::AsNumber(Digit));
}

void UPGDigitButtonWidget::HandleDigitClicked()
{
	OnDigitClicked.Broadcast(Digit);
}
