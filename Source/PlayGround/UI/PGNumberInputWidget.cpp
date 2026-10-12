// Fill out your copyright notice in the Description page of Project Settings.

#include "PGNumberInputWidget.h"
#include "Components/Button.h"
#include "../PlayerController/PGPlayerController.h"
#include "Components/UniformGridPanel.h"
#include "PGDigitButtonWidget.h"
#include "Components/TextBlock.h"

void UPGNumberInputWidget::AppendDigit(int32 Digit)
{
	UE_LOG(LogTemp, Log, TEXT("Clicked digit: %d"), Digit);

	if (Digit < 1 || Digit > 9)
	{
		return;
	}

	if (CurrentInput.Len() >= 3)
	{
		ShowInputError(FText::FromString(TEXT("숫자는 최대 3자리까지 입력할 수 있습니다.")));
		return;
	}

	FString DigitString = FString::FromInt(Digit);

	if (CurrentInput.Contains(DigitString))
	{
		ShowInputError(FText::FromString(TEXT("중복되지 않은 숫자를 입력해주세요.")));
		return;
	}

	CurrentInput += DigitString;
	ShowInputError(FText::GetEmpty());

	if (!IsValid(TextBlock_Input))
	{
		return;
	}

	TextBlock_Input->SetText(FText::FromString(CurrentInput));

	UE_LOG(LogTemp, Log, TEXT("Current input: %s"), *CurrentInput);
}

void UPGNumberInputWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (!IsValid(UniformGridPanel_Digits))
	{
		return;
	}

	if (!IsValid(Button_Delete))
	{
		return;
	}

	Button_Delete->OnClicked.AddDynamic(
		this,
		&UPGNumberInputWidget::HandleDeleteClicked
	);

	if (!IsValid(Button_Submit))
	{
		return;
	}


	Button_Submit->OnClicked.AddDynamic(
		this,
		&UPGNumberInputWidget::HandleSubmitClicked
	);


	for (UWidget* Child : UniformGridPanel_Digits->GetAllChildren())
	{
		UPGDigitButtonWidget* DigitButton = Cast<UPGDigitButtonWidget>(Child);

		if (!IsValid(DigitButton))
		{
			continue;
		}

		DigitButton->OnDigitClicked.AddUObject(
			this,
			&UPGNumberInputWidget::AppendDigit
		);
	}
}

void UPGNumberInputWidget::HandleDeleteClicked()
{

	if (CurrentInput.IsEmpty())
	{
		return;
	}

	CurrentInput.LeftChopInline(1);

	if (IsValid(TextBlock_Input))
	{
		TextBlock_Input->SetText(FText::FromString(CurrentInput));
	}

	ShowInputError(FText::GetEmpty());

}

void UPGNumberInputWidget::HandleSubmitClicked()
{
	APGPlayerController* PGPlayerController = GetOwningPlayer<APGPlayerController>();

	if (!IsValid(PGPlayerController))
	{
		return;
	}

	if (CurrentInput.Len() != 3)
	{
		ShowInputError(FText::FromString(TEXT("숫자 3자리를 입력해주세요.")));
		return;
	}

	PGPlayerController->ServerRPCSubmitGuess(CurrentInput);
	ClearInput();
}

void UPGNumberInputWidget::ClearInput()
{
	CurrentInput.Empty();
	ShowInputError(FText::GetEmpty());
	if (!IsValid(TextBlock_Input))
	{
		return;
	}

	TextBlock_Input->SetText(FText::FromString(CurrentInput));
	
}

void UPGNumberInputWidget::ShowInputError(const FText& Message)
{
	if (!IsValid(TextBlock_Error))
	{
		return;
	}

	TextBlock_Error->SetText(Message);
}
