// Fill out your copyright notice in the Description page of Project Settings.


#include "PGResultWidget.h"
#include "Components/TextBlock.h"

void UPGResultWidget::SetResultMessage(const FText& Message)
{
	if (!IsValid(TextBlock_Result))
	{
		return;
	}

	TextBlock_Result->SetText(Message);
}
