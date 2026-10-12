// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "../GameType/PGGameTypes.h"
#include "PGGameState.generated.h"

class APGPlayerState;
/**
 * 
 */
DECLARE_MULTICAST_DELEGATE_OneParam(
	FOnGamePhaseChanged,
	EGamePhase
);

DECLARE_MULTICAST_DELEGATE_OneParam(
	FOnJudgeResultChanged,
	const FString&
);

DECLARE_MULTICAST_DELEGATE_OneParam(
	FOnGuessHistoryChanged,
	const TArray<FGuessRecord>&
);

DECLARE_MULTICAST_DELEGATE(FOnWinnerChanged);

DECLARE_MULTICAST_DELEGATE(FOnCurrentTurnChanged);

UCLASS()
class PLAYGROUND_API APGGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	//Property Replication 등록
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// 현 게임 단계 반환
	UFUNCTION()
	EGamePhase GetGamePhase() const;
	
	// 단계 변경
	UFUNCTION()
	void SetGamePhase(EGamePhase NewPhase);

	//승자 getter
	UFUNCTION()
	APGPlayerState* GetWinnerPlayerState() const;

	//승자 setter
	UFUNCTION()
	void SetWinnerPlayerState(APGPlayerState* NewWinner);

	// 판정 getter
	UFUNCTION()
	FString GetLastJudgeResult() const;

	// 판정 setter
	UFUNCTION()
	void SetLastJudgeResult(const FString& NewResult);

	UFUNCTION()
	void OnRep_CurrentGamePahse();

	UFUNCTION()
	void OnRep_LastJudgeResult();

	UFUNCTION()
	void OnRep_GuessHistory();

	UFUNCTION()
	void OnRep_WinnerPlayerState();

	UFUNCTION()
	void OnRep_CurrentTurnPlayerState();

	FOnGamePhaseChanged OnGamePhaseChanged;

	FOnJudgeResultChanged OnJudgeResultChanged;

	FOnGuessHistoryChanged OnGuessHistoryChanged;

	FOnWinnerChanged OnWinnerChanged;

	FOnCurrentTurnChanged OnCurrentTurnChanged;

	void AddGuessRecord(const FGuessRecord& NewRecord);

	void ClearGuessHistory();

	const TArray<FGuessRecord>& GetGuessHistory() const;

	APGPlayerState* GetCurrentTurnPlayerState() const;

	void SetCurrentTurnPlayerState(APGPlayerState* NewTurnPlayer);

	double GetTurnEndTime() const;

	void SetTurnEndTime(double NewEndTime);

private:
	// 현재 게임 상태
	UPROPERTY(ReplicatedUsing = OnRep_CurrentGamePahse)
	EGamePhase CurrentGamePhase = EGamePhase::Waiting;

	// 승자 플레이어
	UPROPERTY(ReplicatedUsing = OnRep_WinnerPlayerState)
	TObjectPtr<APGPlayerState> WinnerPlayerState = nullptr;

	// 판정 저장
	UPROPERTY(ReplicatedUsing = OnRep_LastJudgeResult)
	FString LastJudgeResult;

	UPROPERTY(ReplicatedUsing = OnRep_GuessHistory)
	TArray<FGuessRecord> GuessHistory;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentTurnPlayerState)
	TObjectPtr<APGPlayerState> CurrentTurnPlayerState = nullptr;

	UPROPERTY(Replicated)
	double TurnEndTime = 0.0;
};
