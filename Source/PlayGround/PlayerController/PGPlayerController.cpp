// Fill out your copyright notice in the Description page of Project Settings.


#include "PGPlayerController.h"
#include "../PlayerState/PGPlayerState.h"
#include "../GameMode/PGGameMode.h"
#include "../GameState/PGGameState.h"
#include "../UI/PGGameHUDWidget.h"

void APGPlayerController::ServerRPCSetReady_Implementation(bool bNewReady)
{
	UE_LOG(LogTemp, Warning, TEXT("Ready RPC entered"));

	APGPlayerState* PGPlayerState = GetPlayerState<APGPlayerState>();

	if (!IsValid(PGPlayerState))
	{
		return;
	}

	APGGameState* CurrentGameState = GetWorld()->GetGameState<APGGameState>();

	if (!IsValid(CurrentGameState))
	{
		return;
	}

	if (CurrentGameState->GetGamePhase() != EGamePhase::Waiting)
	{
		return;
	}

	PGPlayerState->SetReady(bNewReady);

	UE_LOG(LogTemp, Log, TEXT("Ready request applied: %s"),
		bNewReady ? TEXT("true") : TEXT("false"));

	APGGameMode* PGGameMode = GetWorld()->GetAuthGameMode<APGGameMode>();

	if (!IsValid(PGGameMode))
	{
		return;
	}

	PGGameMode->TryStartGame();
}

void APGPlayerController::ServerRPCSubmitGuess_Implementation(const FString& SubmittedNumber)
{
	APGPlayerState* PGPlayerState = GetPlayerState<APGPlayerState>();

	if (!IsValid(PGPlayerState))
	{
		return;
	}

	APGGameMode* PGGameMode = GetWorld()->GetAuthGameMode<APGGameMode>();

	if (!IsValid(PGGameMode))
	{
		return;
	}

	PGGameMode->SubmitGuess(PGPlayerState,SubmittedNumber);
}

void APGPlayerController::ClientRPCNotifyGuessValidation_Implementation(EGuessValidationResult ValidationResult)
{
	if (!IsValid(GameHUDWidgetInstance))
	{
		return;
	}

	switch (ValidationResult)
	{
	case EGuessValidationResult::Valid:
		break;
	case EGuessValidationResult::InvalidLength:
		GameHUDWidgetInstance->ShowInputError(FText::FromString(TEXT("숫자 3자리를 입력해주세요.")));
		break;
	case EGuessValidationResult::InvalidCharacter:
		GameHUDWidgetInstance->ShowInputError(FText::FromString(TEXT("옳바른 숫자를 입력해주세요.")));
		break;
	case EGuessValidationResult::DuplicateDigit:
		GameHUDWidgetInstance->ShowInputError(FText::FromString(TEXT("중복되지 않은 숫자를 입력해주세요.")));
		break;
	default:
		break;
	}
}

void APGPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (!IsLocalController())
	{
		return;
	}

	GameHUDWidgetInstance = CreateWidget<UPGGameHUDWidget>(this, GameHUDWidgetClass);

	if (!IsValid(GameHUDWidgetInstance))
	{
		return;
	}

	GameHUDWidgetInstance->AddToViewport();

	bShowMouseCursor = true;

	FInputModeUIOnly InputMode;
	SetInputMode(InputMode);
}

void APGPlayerController::ClientRPCReceiveChatMessage_Implementation(const FString& Message)
{
	if (!IsValid(GameHUDWidgetInstance))
	{
		return;
	}

	GameHUDWidgetInstance->AppendChatMessage(Message);
}

void APGPlayerController::ServerRPCSendChatMessage_Implementation(const FString& Message)
{
	APGPlayerState* PGPlayerState = GetPlayerState<APGPlayerState>();

	if (!IsValid(PGPlayerState))
	{
		return;
	}

	APGGameMode* PGGameMode = GetWorld()->GetAuthGameMode<APGGameMode>();

	if (!IsValid(PGGameMode))
	{
		return;
	}

	PGGameMode->BroadcastChatMessage(PGPlayerState, Message);
}
