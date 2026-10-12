// Fill out your copyright notice in the Description page of Project Settings.

#include "PGScoreboardWidget.h"
#include "Components/TextBlock.h"
#include "../UI/PGGuessRecordWidget.h"
#include "Components/VerticalBox.h"

void UPGScoreboardWidget::SetJudgeResult(const FString& Result)
{
	if (!IsValid(TextBlock_JudgeResult))
	{
		return;
	}

	TextBlock_JudgeResult->SetText(FText::FromString(Result));
}

void UPGScoreboardWidget::SetGuessHistory(const TArray<FGuessRecord>& Records)
{
	UE_LOG(
		LogTemp,
		Warning,
		TEXT("Scoreboard history: %d, Box valid: %d, Class set: %d"),
		Records.Num(),
		IsValid(VerticalBox_GuessHistory),
		GuessRecordWidgetClass != nullptr
	);

	UE_LOG(
		LogTemp,
		Warning,
		TEXT("Scoreboard class: %s, Record class: %s"),
		*GetClass()->GetPathName(),
		*GetNameSafe(GuessRecordWidgetClass.Get())
	);

	if (!IsValid(VerticalBox_GuessHistory))
	{
		return;
	}

	VerticalBox_GuessHistory->ClearChildren();

	if (!GuessRecordWidgetClass)
	{
		return;
	}

	for (const FGuessRecord& Record : Records)
	{
		UPGGuessRecordWidget* RecordWidget = CreateWidget<UPGGuessRecordWidget>(GetOwningPlayer(), GuessRecordWidgetClass);

		if (!IsValid(RecordWidget))
		{
			continue;
		}

		RecordWidget->SetRecord(Record);

		VerticalBox_GuessHistory->AddChildToVerticalBox(RecordWidget);

	}
}

void UPGScoreboardWidget::SetRemainingTime(int32 RemainingSeconds)
{
	if (!IsValid(TextBlock_RemainingTime))
	{
		return;
	}

	TextBlock_RemainingTime->SetText(FText::AsNumber(RemainingSeconds));
}


