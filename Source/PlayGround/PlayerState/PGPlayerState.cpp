// Fill out your copyright notice in the Description page of Project Settings.


#include "PGPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "../GameMode/PGGameMode.h"

void APGPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, AttemptCount);
	DOREPLIFETIME(ThisClass, bIsReady);
}

int32 APGPlayerState::GetAttemptCount() const
{
    return AttemptCount;
}

int32 APGPlayerState::GetMaxAttemptCount() const
{
	return MaxAttemptCount;
}

void APGPlayerState::IncreaseAttemptCount()
{
	// 클라에서는 변경 불가
	if (!HasAuthority())
	{
		return;
	}

	if (AttemptCount < MaxAttemptCount)
	{
		++AttemptCount;
		OnAttemptCountChanged.Broadcast(AttemptCount, MaxAttemptCount);
	}
}

bool APGPlayerState::CanAttempt() const
{
	return AttemptCount < MaxAttemptCount;
}

void APGPlayerState::ResetAttemptCount()
{
	if (!HasAuthority())
	{
		return;
	}

	AttemptCount = 0;

	OnAttemptCountChanged.Broadcast(AttemptCount,MaxAttemptCount);
}

void APGPlayerState::SetReady(bool bNewReady)
{
	if (!HasAuthority())
	{
		return;
	}

	bIsReady = bNewReady;
}


void APGPlayerState::OnRep_AttemptCount()
{
	OnAttemptCountChanged.Broadcast(AttemptCount,MaxAttemptCount);
}
