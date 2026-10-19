// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/LobbyDisplayName.h"

#include "Components/TextBlock.h"

void ULobbyDisplayName::SetDisplayName(const FText& NewDisplayName) const
{
	DisplayNameText->SetText(NewDisplayName);
}
