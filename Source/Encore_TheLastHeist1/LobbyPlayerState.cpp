// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerState.h"

#include "ThiefPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "LobbyThiefDummy.h"
#include "EngineUtils.h"
#include "LobbyPlayerController.h"
#include "Widgets/LobbyDisplayName.h"

void ALobbyPlayerState::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);

	AThiefPlayerState* NewPlayerState = Cast<AThiefPlayerState>(PlayerState);

	if (IsValid(NewPlayerState))
	{
		NewPlayerState->SetDisplayName(DisplayName);
	}
}

void ALobbyPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyPlayerState, DisplayName);
	DOREPLIFETIME(ALobbyPlayerState, IsReady);
}

void ALobbyPlayerState::OnRep_DisplayName()
{
	ALobbyPlayerController* PC = Cast<ALobbyPlayerController>(GetPlayerController());
	if (!PC) return;
	if (!PC->IsLocalController()) return;
	
	PC->LobbyDisplayNameWidget->SetDisplayName(DisplayName);
}
