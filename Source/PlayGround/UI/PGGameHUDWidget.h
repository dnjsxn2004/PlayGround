// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../GameType/PGGameTypes.h"
#include "PGGameHUDWidget.generated.h"

class UButton;
class UPGNumberInputWidget;
class UPGScoreboardWidget;
class UPGResultWidget;
class UPGChattingWidget;
/**
 * 
 */
UCLASS()
class PLAYGROUND_API UPGGameHUDWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	// 클릭 처리
	UFUNCTION()
	void HandleReadyButtonClicked();

	void HandleGamePhaseChanged(EGamePhase NewPhase);

	void ShowInputError(const FText& Message);

	void HandleJudgeResultChanged(const FString& Result);

	void HandleGuessHistoryChanged(const TArray<FGuessRecord>& Records);

	void RefreshRoundResult();

	void RefreshRemainingTime();

	void RefreshNumberInputEnabled();

	void AppendChatMessage(const FString& Message);

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Ready;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPGNumberInputWidget> NumberInputWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPGScoreboardWidget> ScoreboardWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPGResultWidget> ResultWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPGChattingWidget> ChattingWidget;

	FTimerHandle RemainingTimeRefreshTimerHandle;
	
};
