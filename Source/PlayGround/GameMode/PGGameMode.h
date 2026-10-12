// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "../GameType/PGGameTypes.h"
#include "PGGameMode.generated.h"

class APGPlayerState;
/**
 * 
 */
UCLASS()
class PLAYGROUND_API APGGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	APGGameMode();

	// 정답 숫자 생성
	UFUNCTION()
	FString GenerateCorrectNumber();

	// 정답 검수 후 반환(판정 x, 제출 유효 여부만 판단)
	UFUNCTION()
	EGuessValidationResult IsValidNumber(const FString& SubmittedNumber);

	// 판정 로직
	UFUNCTION()
	FString JudgeNumber(const FString& SecretNumber,const FString& SubmittedNumber);

	// 제출 처리 함수
	void SubmitGuess(APGPlayerState* SubmittingPlayerState, const FString& SubmittedNumber);

private:
	// 정답 문자열
	UPROPERTY()
	FString CorrectNumber;

public:
	// 새 게임
	void StartNewRound();

	// 새 게임 시도
	void TryStartGame();

	// 무승부 판정
	UFUNCTION()
	bool AreAllAttemptsExhausted() const;

	// 라운드 종료
	UFUNCTION()
	void EndRound(APGPlayerState* Winner);

	// 턴 넘기기
	void AdvanceTurn();

	void BroadcastChatMessage(APGPlayerState* SendPlayerState, const FString& Message);

private:
	FTimerHandle RoundRestartTimerHandle;

	UPROPERTY(EditDefaultsOnly, Category = "Round")
	float RoundRestartDelay = 3.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Trun", meta = (ClmapMin ="1.0"))
	float TurnDuration = 10.0f;
	
	FTimerHandle TurnTimerHandle;

	void StartTurnTimer();

	void HandleTurnTimeout();
};
