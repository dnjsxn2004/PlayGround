// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../GameType/PGGameTypes.h"
#include "PGGuessRecordWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class PLAYGROUND_API UPGGuessRecordWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetRecord(const FGuessRecord& Record);

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_PlayerName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_SubmittedNumber;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_JudgeResult;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_AttemptCount;
	
};
