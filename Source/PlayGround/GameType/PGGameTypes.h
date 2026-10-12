#pragma once

#include "CoreMinimal.h"
#include "PGGameTypes.generated.h"

UENUM(BlueprintType)
enum class EGuessValidationResult : uint8
{
	Valid,				// 정상
	InvalidLength,		// 자릿수 틀림
	InvalidCharacter,	// 1~9 이외의 문자
	DuplicateDigit		// 중복 숫자
};

UENUM(BlueprintType)
enum class EGamePhase : uint8
{
	Waiting,	// 플레이 대기
	Playing,	// 게임중
	RoundOver	// 게임 끝(결과)
};

USTRUCT()
struct FGuessRecord
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FString PlayerName;

	UPROPERTY()
	FString SubmittedNumber;

	UPROPERTY()
	FString JudgeResult;

	UPROPERTY()
	int32 AttemptCount = 0;

	UPROPERTY()
	int32 MaxAttemptCount = 0;

};