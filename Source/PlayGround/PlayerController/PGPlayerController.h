// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "../GameType/PGGameTypes.h"
#include "PGPlayerController.generated.h"

class APGPlayerState;
class APGGameMode;
class UPGGameHUDWidget;

/**
 * 
 */
UCLASS()
class PLAYGROUND_API APGPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
public:
	UFUNCTION(Server, Reliable)
	void ServerRPCSetReady(bool bNewReady);

	UFUNCTION(Server, Reliable)
	void ServerRPCSubmitGuess(const FString& SubmittedNumber);

	UFUNCTION(Client, Reliable)
	void ClientRPCNotifyGuessValidation(EGuessValidationResult ValidationResult);

	UFUNCTION(Server, Reliable)
	void ServerRPCSendChatMessage(const FString& Message);

	UFUNCTION(Client, Reliable)
	void ClientRPCReceiveChatMessage(const FString& Message);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UPGGameHUDWidget> GameHUDWidgetClass;

	UPROPERTY()
	TObjectPtr<UPGGameHUDWidget> GameHUDWidgetInstance;

};
