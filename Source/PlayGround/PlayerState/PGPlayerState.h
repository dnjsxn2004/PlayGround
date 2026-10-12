// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PGPlayerState.generated.h"

/**
 * 
 */

DECLARE_MULTICAST_DELEGATE_TwoParams(
	FOnAttemptCountChanged,
	int32,  // 현재 시도 횟수
	int32   // 최대 시도 횟수
);

UCLASS()
class PLAYGROUND_API APGPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	//Property Replication 등록
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	// 시도 횟수 반환
	int32 GetAttemptCount() const;

	// 최대 횟수 반환
	int32 GetMaxAttemptCount() const;

	// 시도 횟수 증가 함수
	void IncreaseAttemptCount();

	// 시도 횟수 확인
	bool CanAttempt() const;

	// 횟수 초기화
	void ResetAttemptCount();

	// 준비상태
	bool IsReady() const { return bIsReady; }

	// 준비 상태 변경
	void SetReady(bool bNewReady);

	FOnAttemptCountChanged OnAttemptCountChanged;

private:
	// 현재 시도 횟수
	UPROPERTY(ReplicatedUsing = OnRep_AttemptCount)
	int32 AttemptCount = 0;

	// 최대 시도 횟수
	UPROPERTY()
	int32 MaxAttemptCount = 3;

	UPROPERTY(Replicated)
	bool bIsReady = false;

	UFUNCTION()
	void OnRep_AttemptCount();
	
};
