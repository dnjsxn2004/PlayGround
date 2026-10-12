
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../GameType/PGGameTypes.h"
#include "PGScoreboardWidget.generated.h"

class UTextBlock;
class UVerticalBox;
class UPGGuessRecordWidget;

/**
 * 
 */
UCLASS()
class PLAYGROUND_API UPGScoreboardWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void SetJudgeResult(const FString& Result);

	void SetGuessHistory(const TArray<FGuessRecord>& Records);

	void SetRemainingTime(int32 RemainingSeconds);

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_JudgeResult;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> VerticalBox_GuessHistory;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UPGGuessRecordWidget> GuessRecordWidgetClass;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_RemainingTime;
};
