// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PGResultWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class PLAYGROUND_API UPGResultWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetResultMessage(const FText& Message);

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Result;
	
};
