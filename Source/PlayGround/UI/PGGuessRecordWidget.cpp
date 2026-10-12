// Fill out your copyright notice in the Description page of Project Settings.


#include "PGGuessRecordWidget.h"
#include "Components/TextBlock.h"

void UPGGuessRecordWidget::SetRecord(const FGuessRecord& Record)
{
	if (IsValid(TextBlock_PlayerName))
	{
		TextBlock_PlayerName->SetText(FText::FromString(Record.PlayerName));
	}

	if (IsValid(TextBlock_SubmittedNumber))
	{
		TextBlock_SubmittedNumber->SetText(FText::FromString(Record.SubmittedNumber));
	}

	if (IsValid(TextBlock_JudgeResult))
	{
		TextBlock_JudgeResult->SetText(FText::FromString(Record.JudgeResult));
	}

	if (IsValid(TextBlock_AttemptCount))
	{
		FString AttemptCount = FString::Printf(
			TEXT("%d / %d"),
			Record.AttemptCount,
			Record.MaxAttemptCount
		);

		TextBlock_AttemptCount->SetText(FText::FromString(AttemptCount));
	}

}
