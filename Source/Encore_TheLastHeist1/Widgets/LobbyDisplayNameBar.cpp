// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/LobbyDisplayNameBar.h"

#include "LobbyPlayerController.h"
#include "LobbyPlayerState.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"




void ULobbyDisplayNameBar::Setup(ALobbyPlayerState* InPlayerState)
{
	if (!IsValid(InPlayerState)) return;
	
	PlayerState = InPlayerState;
	
	ALobbyPlayerController* PC = Cast<ALobbyPlayerController>(GetOwningPlayer());
			
	if (IsValid(PC) && PC->GetPlayerState<ALobbyPlayerState>() == PlayerState)
	{
		SetBarColor(FLinearColor(0.0f, 0.85f, 0.0f, 0.5f));
	}
	else
	{
		SetBarColor(FLinearColor(0.086f, 0.086f, 0.086f, 0.5f));
	}
	
	UpdateBar();
}

void ULobbyDisplayNameBar::UpdateBar()
{
	if (!IsValid(PlayerState)) return;

	SetPlayerName(PlayerState->DisplayName);
}


void ULobbyDisplayNameBar::SetPlayerName(const FText& PlayerName) const
{
	if (!PlayerNameValueText) return;
	
	PlayerNameValueText->SetText(PlayerName);
}

void ULobbyDisplayNameBar::SetBarColor(const FLinearColor Color) const
{
	if (!PlayerBar) return;
	
	PlayerBar->SetBrushColor(Color);
}
