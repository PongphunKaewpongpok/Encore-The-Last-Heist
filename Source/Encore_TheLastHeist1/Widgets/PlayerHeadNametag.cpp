// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/PlayerHeadNametag.h"

#include "Components/TextBlock.h"

void UPlayerHeadNametag::SetDisplayName(const FText& NewName)
{
	if (DisplayNameText)
	{
		DisplayNameText->SetText(NewName);
	}
}