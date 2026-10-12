// Fill out your copyright notice in the Description page of Project Settings.


#include "PGGameState.h"
#include "Net/UnrealNetwork.h"
#include "../PlayerState/PGPlayerState.h"
#include "../UI/PGScoreboardWidget.h"

void APGGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, CurrentGamePhase);
	DOREPLIFETIME(ThisClass, WinnerPlayerState);
	DOREPLIFETIME(ThisClass, LastJudgeResult);
	DOREPLIFETIME(ThisClass, GuessHistory);
	DOREPLIFETIME(ThisClass, CurrentTurnPlayerState);
	DOREPLIFETIME(ThisClass, TurnEndTime);
}

EGamePhase APGGameState::GetGamePhase() const
{
	return CurrentGamePhase;
}

void APGGameState::SetGamePhase(EGamePhase NewPhase)
{
	if (!HasAuthority() || CurrentGamePhase == NewPhase)
	{
		return;
	}

	CurrentGamePhase = NewPhase;
	OnGamePhaseChanged.Broadcast(CurrentGamePhase);
}

APGPlayerState* APGGameState::GetWinnerPlayerState() const
{
	return WinnerPlayerState;
}

void APGGameState::SetWinnerPlayerState(APGPlayerState* NewWinner)
{
	if (!HasAuthority())
	{
		return;
	}

	WinnerPlayerState = NewWinner;
	OnWinnerChanged.Broadcast();
}

FString APGGameState::GetLastJudgeResult() const
{
	return LastJudgeResult;
}

void APGGameState::SetLastJudgeResult(const FString& NewResult)
{
	if (!HasAuthority())
	{
		return;
	}

	LastJudgeResult = NewResult;
	OnJudgeResultChanged.Broadcast(NewResult);
}

void APGGameState::OnRep_CurrentGamePahse()
{
	OnGamePhaseChanged.Broadcast(CurrentGamePhase);
}

void APGGameState::OnRep_LastJudgeResult()
{
	OnJudgeResultChanged.Broadcast(LastJudgeResult);
}

void APGGameState::OnRep_GuessHistory()
{
	OnGuessHistoryChanged.Broadcast(GuessHistory);
}

void APGGameState::OnRep_WinnerPlayerState()
{
	OnWinnerChanged.Broadcast();
}

void APGGameState::OnRep_CurrentTurnPlayerState()
{
	OnCurrentTurnChanged.Broadcast();
}

void APGGameState::AddGuessRecord(const FGuessRecord& NewRecord)
{
	if (!HasAuthority())
	{
		return;
	}

	GuessHistory.Add(NewRecord);
	UE_LOG(LogTemp, Warning, TEXT("History added: %d"), GuessHistory.Num());
	OnGuessHistoryChanged.Broadcast(GuessHistory);
}

void APGGameState::ClearGuessHistory()
{
	if (!HasAuthority())
	{
		return;
	}

	GuessHistory.Empty();
	OnGuessHistoryChanged.Broadcast(GuessHistory);
}

const TArray<FGuessRecord>& APGGameState::GetGuessHistory() const
{
	return GuessHistory;
}

APGPlayerState* APGGameState::GetCurrentTurnPlayerState() const
{
	return CurrentTurnPlayerState;
}

void APGGameState::SetCurrentTurnPlayerState(APGPlayerState* NewTurnPlayer)
{
	if (!HasAuthority())
	{
		return;
	}

	CurrentTurnPlayerState = NewTurnPlayer;
	OnCurrentTurnChanged.Broadcast();
}

double APGGameState::GetTurnEndTime() const
{
	return TurnEndTime;
}

void APGGameState::SetTurnEndTime(double NewEndTime)
{
	if (!HasAuthority())
	{
		return;
	}

	TurnEndTime = NewEndTime;
}
