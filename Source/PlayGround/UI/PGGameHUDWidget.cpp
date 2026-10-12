// Fill out your copyright notice in the Description page of Project Settings.


#include "PGGameHUDWidget.h"
#include "../PlayerController/PGPlayerController.h"
#include "Components/Button.h"
#include "../GameState/PGGameState.h"
#include "../UI/PGNumberInputWidget.h"
#include "../UI/PGScoreboardWidget.h"
#include "../UI/PGResultWidget.h"
#include "../PlayerState/PGPlayerState.h"
#include "TimerManager.h"
#include "PGChattingWidget.h"

void UPGGameHUDWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (IsValid(Button_Ready))
	{
		Button_Ready->OnClicked.AddDynamic(
			this,
			&UPGGameHUDWidget::HandleReadyButtonClicked
		);
	}

	APGGameState* CurrentGameState = GetWorld()->GetGameState<APGGameState>();

	if (!IsValid(CurrentGameState))
	{
		return;
	}

	CurrentGameState->OnGamePhaseChanged.AddUObject(
		this,
		&UPGGameHUDWidget::HandleGamePhaseChanged
	);

	CurrentGameState->OnJudgeResultChanged.AddUObject(
		this,
		&UPGGameHUDWidget::HandleJudgeResultChanged
	);

	CurrentGameState->OnGuessHistoryChanged.AddUObject(
		this,
		&UPGGameHUDWidget::HandleGuessHistoryChanged
	);

	CurrentGameState->OnWinnerChanged.AddUObject(
		this,
		&UPGGameHUDWidget::RefreshRoundResult
	);

	CurrentGameState->OnCurrentTurnChanged.AddUObject(
		this,
		&UPGGameHUDWidget::RefreshNumberInputEnabled
	);

	HandleGamePhaseChanged(CurrentGameState->GetGamePhase());
	HandleJudgeResultChanged(CurrentGameState->GetLastJudgeResult());
	HandleGuessHistoryChanged(CurrentGameState->GetGuessHistory());
	RefreshRoundResult();
	RefreshNumberInputEnabled();
}

void UPGGameHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	RefreshRemainingTime();

	GetWorld()->GetTimerManager().SetTimer(
		RemainingTimeRefreshTimerHandle,
		this,
		&UPGGameHUDWidget::RefreshRemainingTime,
		0.1f,
		true
	);
}

void UPGGameHUDWidget::NativeDestruct()
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(RemainingTimeRefreshTimerHandle);
	}

	Super::NativeDestruct();
}

void UPGGameHUDWidget::HandleReadyButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Ready button clicked"));

	APGPlayerController* PGPlayerController = GetOwningPlayer<APGPlayerController>();

	if (!IsValid(PGPlayerController))
	{
		return;
	}

	if (!PGPlayerController->IsLocalController())
	{
		return;
	}

	PGPlayerController->ServerRPCSetReady(true);
}

void UPGGameHUDWidget::HandleGamePhaseChanged(EGamePhase NewPhase)
{
	RefreshRoundResult();
	RefreshNumberInputEnabled();

	APGPlayerController* PGPlayerController = GetOwningPlayer<APGPlayerController>();

	if (!IsValid(PGPlayerController) || !PGPlayerController->IsLocalController())
	{
		return;
	}

	if (IsValid(NumberInputWidget))
	{
		if (NewPhase == EGamePhase::Playing)
		{
			NumberInputWidget->ClearInput();
		}
	}

	if (!IsValid(Button_Ready))
	{
		return;
	}

	if (NewPhase == EGamePhase::Waiting)
	{
		Button_Ready->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Button_Ready->SetVisibility(ESlateVisibility::Collapsed);
	}

}

void UPGGameHUDWidget::ShowInputError(const FText& Message)
{
	if (!IsValid(NumberInputWidget))
	{
		return;
	}

	NumberInputWidget->ShowInputError(Message);
}

void UPGGameHUDWidget::HandleJudgeResultChanged(const FString& Result)
{
	if (!IsValid(ScoreboardWidget))
	{
		return;
	}

	ScoreboardWidget->SetJudgeResult(Result);
}

void UPGGameHUDWidget::HandleGuessHistoryChanged(const TArray<FGuessRecord>& Records)
{
	UE_LOG(LogTemp, Warning, TEXT("HUD history received: %d"), Records.Num());

	if (!IsValid(ScoreboardWidget))
	{
		return;
	}

	ScoreboardWidget->SetGuessHistory(Records);
}

void UPGGameHUDWidget::RefreshRoundResult()
{
	if (!IsValid(ResultWidget))
	{
		return;
	}

	APGGameState* CurrentState = GetWorld()->GetGameState<APGGameState>();

	if (!IsValid(CurrentState))
	{
		return;
	}

	if (CurrentState->GetGamePhase() != EGamePhase::RoundOver)
	{
		ResultWidget->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}
	
	APGPlayerState* Winner = CurrentState->GetWinnerPlayerState();

	if (IsValid(Winner))
	{
		FText WinnerMessage = FText::FromString(FString::Printf(TEXT("%s 승리"), *Winner->GetPlayerName()));

		ResultWidget->SetResultMessage(WinnerMessage);
	}
	else
	{
		ResultWidget->SetResultMessage(FText::FromString(TEXT("무승부")));
	}

	ResultWidget->SetVisibility(ESlateVisibility::Visible);
}

void UPGGameHUDWidget::RefreshRemainingTime()
{
	if (!IsValid(ScoreboardWidget))
	{
		return;
	}

	APGGameState* CurrentGameState = GetWorld()->GetGameState<APGGameState>();

	if (CurrentGameState->GetGamePhase() != EGamePhase::Playing)
	{
		ScoreboardWidget->SetRemainingTime(0);
		return;
	}

	const double RemainingTime = CurrentGameState->GetTurnEndTime() - CurrentGameState->GetServerWorldTimeSeconds();

	const int32 RemainingSeconds = FMath::CeilToInt(FMath::Max(0.0, RemainingTime));

	ScoreboardWidget->SetRemainingTime(RemainingTime);
}

void UPGGameHUDWidget::RefreshNumberInputEnabled()
{
	if (!IsValid(NumberInputWidget))
	{
		return;
	}

	NumberInputWidget->SetIsEnabled(false);

	APGGameState* CurrentGameState = GetWorld()->GetGameState<APGGameState>();

	if (!IsValid(CurrentGameState))
	{
		return;
	}

	APGPlayerController* PGPlayerController = GetOwningPlayer<APGPlayerController>();

	if (!IsValid(PGPlayerController))
	{
		return;
	}

	APGPlayerState* PGPlayerState = PGPlayerController->GetPlayerState<APGPlayerState>();

	if (!IsValid(PGPlayerState))
	{
		return;
	}

	const bool bCanInput = CurrentGameState->GetGamePhase() == EGamePhase::Playing 
		&& CurrentGameState->GetCurrentTurnPlayerState() == PGPlayerState;

	NumberInputWidget->SetIsEnabled(bCanInput);
}

void UPGGameHUDWidget::AppendChatMessage(const FString& Message)
{
	if (!IsValid(ChattingWidget))
	{
		return;
	}

	ChattingWidget->AppendChatMessage(Message);
}
