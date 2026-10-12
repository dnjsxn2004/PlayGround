// Fill out your copyright notice in the Description page of Project Settings.


#include "PGGameMode.h"
#include "../PlayerState/PGPlayerState.h"
#include "../GameType/PGGameTypes.h"
#include "GameFramework/GameStateBase.h"
#include "../GameState/PGGameState.h"
#include "TimerManager.h"
#include "../PlayerController/PGPlayerController.h"

APGGameMode::APGGameMode()
{
	PlayerStateClass = APGPlayerState::StaticClass();
}

/*
1~9 까지 랜덤 난수 생성 후 문자열로 변화
현재까지 저장된 SaveNumber 안에 NumberToString 의 숫자가 없으면 SaveNumber 문자열에 추가
SaveNumber 의 길이가 3일때까지 반복
*/
FString APGGameMode::GenerateCorrectNumber()
{
	// 정답 숫자 저장
	FString SaveNumber;

	while (SaveNumber.Len() < 3)
	{
		// 1~9 까지 랜덤 숫자 뽑기
		int32 RandomNumber = FMath::RandRange(1, 9);

		// 정수 문자열로 변환
		FString NumberToString = FString::FromInt(RandomNumber);

		// 중복 검사 ( SaveNumber 안에 NumberToString 이 없을때 실행)
		if (!SaveNumber.Contains(NumberToString))
		{
			// 문자열에 추가
			SaveNumber += NumberToString;
		}
	}

	return SaveNumber;
}

/*
유효 정답인지 검사하는 항목
*/
EGuessValidationResult APGGameMode::IsValidNumber(const FString& SubmittedNumber)
{
	// 세 자리인지 확인
	if (SubmittedNumber.Len() != 3)
	{
		return EGuessValidationResult::InvalidLength;
	}

	FString SaveNumber;

	for (int i = 0; i < SubmittedNumber.Len(); i++)
	{
		TCHAR CheckNumber = SubmittedNumber[i];

		// 허용 문자 검사
		if (CheckNumber < TEXT('1') || CheckNumber > TEXT('9'))
		{
			return EGuessValidationResult::InvalidCharacter;
		}

		int32 FoundIndex = INDEX_NONE;

		// 중복 검사
		if (SaveNumber.FindChar(CheckNumber, FoundIndex))
		{
			return EGuessValidationResult::DuplicateDigit;
		}

		SaveNumber += CheckNumber;

	}

	return EGuessValidationResult::Valid;
}

FString APGGameMode::JudgeNumber(const FString& SecretNumber, const FString& SubmittedNumber)
{
	int32 StrikeCount = 0;
	int32 BallCount = 0;

	for (int i = 0; i < SubmittedNumber.Len(); i++)
	{
		TCHAR CheckNumber = SubmittedNumber[i];
		TCHAR CorrectDigit = SecretNumber[i];

		int FoundIndex = INDEX_NONE;

		if (CheckNumber == CorrectDigit)
		{
			StrikeCount += 1;
		}

		else if (SecretNumber.FindChar(CheckNumber, FoundIndex))
		{
			BallCount += 1;
		}
	}

	if (StrikeCount == 0 && BallCount == 0)
	{
		return TEXT("OUT");
	}

	return FString::Printf(TEXT("%dS%dB"), StrikeCount, BallCount);
}

void APGGameMode::SubmitGuess(APGPlayerState* SubmittingPlayerState, const FString& SubmittedNumber)
{
	APGGameState* CurrentGameState = GetWorld()->GetGameState<APGGameState>();

	if (!IsValid(CurrentGameState))
	{
		return;
	}

	if (CurrentGameState->GetGamePhase() != EGamePhase::Playing)
	{
		return;
	}

	if (!IsValid(SubmittingPlayerState))
	{
		return;
	}

	if (!SubmittingPlayerState->CanAttempt())
	{
		return;
	}

	if (CurrentGameState->GetCurrentTurnPlayerState() != SubmittingPlayerState)
	{
		return;
	}

	const EGuessValidationResult ValidationResult = IsValidNumber(SubmittedNumber);

	if (ValidationResult != EGuessValidationResult::Valid)
	{
		APGPlayerController* PGPlayerController = Cast<APGPlayerController>(SubmittingPlayerState->GetPlayerController());

		if (IsValid(PGPlayerController))
		{
			PGPlayerController->ClientRPCNotifyGuessValidation(ValidationResult);
		}

		return;
	}

	if (CurrentGameState->GetCurrentTurnPlayerState() != SubmittingPlayerState)
	{
		return;
	}

	if (CurrentGameState->GetServerWorldTimeSeconds() >= CurrentGameState->GetTurnEndTime())
	{
		return;
	}

	SubmittingPlayerState->IncreaseAttemptCount();

	FString Result = JudgeNumber(CorrectNumber, SubmittedNumber);

	// 새로운 결과 구조체에 저장
	FGuessRecord NewRecord;
	NewRecord.PlayerName = SubmittingPlayerState->GetPlayerName();
	NewRecord.SubmittedNumber = SubmittedNumber;
	NewRecord.JudgeResult = Result;
	NewRecord.AttemptCount = SubmittingPlayerState->GetAttemptCount();
	NewRecord.MaxAttemptCount = SubmittingPlayerState->GetMaxAttemptCount();
	CurrentGameState->AddGuessRecord(NewRecord);

	CurrentGameState->SetLastJudgeResult(Result);

	if (CorrectNumber == SubmittedNumber)
	{
		EndRound(SubmittingPlayerState);
		return;
	}

	if (AreAllAttemptsExhausted())
	{
		EndRound(nullptr);
		return;
	}

	AdvanceTurn();
}

void APGGameMode::StartNewRound()
{
	APGGameState* CurrentGameState = GetGameState<APGGameState>();
	APGPlayerState* FirstTurnPlayer = nullptr;

	if (!IsValid(CurrentGameState))
	{
		return;
	}

	CorrectNumber = GenerateCorrectNumber();

	for (APlayerState* PlayerState : CurrentGameState->PlayerArray)
	{
		APGPlayerState* PGPlayerState = Cast<APGPlayerState>(PlayerState);

		if (!IsValid(PGPlayerState))
		{
			continue;
		}

		if (FirstTurnPlayer == nullptr)
		{
			FirstTurnPlayer = PGPlayerState;
		}

		PGPlayerState->ResetAttemptCount();
	}
	CurrentGameState->SetWinnerPlayerState(nullptr);
	CurrentGameState->SetLastJudgeResult(TEXT(""));
	CurrentGameState->ClearGuessHistory();
	CurrentGameState->SetCurrentTurnPlayerState(FirstTurnPlayer);
	CurrentGameState->SetGamePhase(EGamePhase::Playing);
	StartTurnTimer();
}

void APGGameMode::TryStartGame()
{
	APGGameState* CurrentGameState = GetGameState<APGGameState>();

	if (!IsValid(CurrentGameState))
	{
		return;
	}

	if (CurrentGameState->GetGamePhase() != EGamePhase::Waiting)
	{
		return;
	}

	if (CurrentGameState->PlayerArray.Num() < 2)
	{
		return;
	}

	for (APlayerState* PlayerState : CurrentGameState->PlayerArray)
	{
		APGPlayerState* PGPlayerState = Cast<APGPlayerState>(PlayerState);

		if (!IsValid(PGPlayerState))
		{
			return;
		}

		if (!PGPlayerState->IsReady())
		{
			return;
		}
	}

	StartNewRound();
}

bool APGGameMode::AreAllAttemptsExhausted() const
{
	APGGameState* CurrentGameState = GetGameState<APGGameState>();

	if (!IsValid(CurrentGameState))
	{
		return false;
	}

	if (CurrentGameState->PlayerArray.Num() == 0)
	{
		return false;
	}

	for (APlayerState* PlayerState : CurrentGameState->PlayerArray)
	{
		APGPlayerState* PGPlayerState = Cast<APGPlayerState>(PlayerState);

		if (!IsValid(PGPlayerState) || PGPlayerState->CanAttempt())
		{
			return false;
		}
	}

	return true;
}

void APGGameMode::EndRound(APGPlayerState* Winner)
{
	APGGameState* CurrentGameStat = GetWorld()->GetGameState<APGGameState>();

	if (!IsValid(CurrentGameStat))
	{
		return;
	}

	if (CurrentGameStat->GetGamePhase() != EGamePhase::Playing)
	{
		return;
	}

	GetWorldTimerManager().ClearTimer(TurnTimerHandle);

	CurrentGameStat->SetWinnerPlayerState(Winner);
	CurrentGameStat->SetGamePhase(EGamePhase::RoundOver);

	GetWorldTimerManager().SetTimer(
		RoundRestartTimerHandle,
		this,
		&APGGameMode::StartNewRound,
		RoundRestartDelay,
		false
	);
}

void APGGameMode::AdvanceTurn()
{
	APGGameState* CurrentGameState = GetGameState<APGGameState>();

	if (!IsValid(CurrentGameState))
	{
		return;
	}

	if (CurrentGameState->GetGamePhase() != EGamePhase::Playing)
	{
		return;
	}

	APGPlayerState* CurrentTurnPlayer = CurrentGameState->GetCurrentTurnPlayerState();

	const int32 CurrentIndex = CurrentGameState->PlayerArray.IndexOfByKey(CurrentTurnPlayer);

	if (CurrentIndex == INDEX_NONE)
	{
		return;
	}

	const int32 PlayerCount = CurrentGameState->PlayerArray.Num();

	if (PlayerCount == 0)
	{
		return;
	}

	for (int offset = 1; offset <= PlayerCount; offset++)
	{
		const int32 NextIndex = (CurrentIndex + offset) % PlayerCount;

		APGPlayerState* NextPlayer = Cast<APGPlayerState>(CurrentGameState->PlayerArray[NextIndex]);

		if (!IsValid(NextPlayer) || !NextPlayer->CanAttempt())
		{
			continue;
		}

		CurrentGameState->SetCurrentTurnPlayerState(NextPlayer);
		StartTurnTimer();
		return;
	}
}

void APGGameMode::BroadcastChatMessage(APGPlayerState* SendPlayerState, const FString& Message)
{
	if (!IsValid(SendPlayerState))
	{
		return;
	}

	FString CleanMessage = Message;

	CleanMessage.TrimStartAndEndInline();

	if (CleanMessage.IsEmpty())
	{
		return;
	}

	APGGameState* CurrentGameState = GetGameState<APGGameState>();

	if (!IsValid(CurrentGameState))
	{
		return;
	}

	const FString FormattedMessage = FString::Printf(TEXT("%s: %s"),*SendPlayerState->GetPlayerName(),*CleanMessage);

	for (APlayerState* PlayerState : CurrentGameState->PlayerArray)
	{
		if (!IsValid(PlayerState))
		{
			continue;
		}

		APGPlayerController* PGPlayerController = Cast<APGPlayerController>(PlayerState->GetPlayerController());

		if (!IsValid(PGPlayerController))
		{
			return;
		}

		PGPlayerController->ClientRPCReceiveChatMessage(FormattedMessage);
	}
}


void APGGameMode::StartTurnTimer()
{
	GetWorldTimerManager().ClearTimer(TurnTimerHandle);

	APGGameState* CurrentGameState = GetGameState<APGGameState>();

	if (!IsValid(CurrentGameState))
	{
		return;
	}

	if (CurrentGameState->GetGamePhase() == EGamePhase::Playing)
	{
		const double EndTime = CurrentGameState->GetServerWorldTimeSeconds() + TurnDuration;

		CurrentGameState->SetTurnEndTime(EndTime);

		GetWorldTimerManager().SetTimer(
			TurnTimerHandle,
			this,
			&APGGameMode::HandleTurnTimeout,
			TurnDuration,
			false
		);
	}


}

void APGGameMode::HandleTurnTimeout()
{
	APGGameState* CurrentGameState = GetGameState<APGGameState>();

	if (!IsValid(CurrentGameState))
	{
		return;
	}

	if (CurrentGameState->GetGamePhase() != EGamePhase::Playing)
	{
		return;
	}

	APGPlayerState* CurrentPlayer = CurrentGameState->GetCurrentTurnPlayerState();

	if (!IsValid(CurrentPlayer))
	{
		return;
	}

	CurrentPlayer->IncreaseAttemptCount();

	if (AreAllAttemptsExhausted())
	{
		EndRound(nullptr);
		return;
	}
	
	AdvanceTurn();
}
