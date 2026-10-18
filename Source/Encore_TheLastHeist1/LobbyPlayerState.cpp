// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerState.h"

#include "ThiefPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "LobbyThiefDummy.h"
#include "EngineUtils.h"

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
	ALobbyThiefDummy* FoundDummy = nullptr;

	for (TActorIterator<ALobbyThiefDummy> It(GetWorld()); It; ++It)
	{
		FoundDummy = *It;
		break;
	}

	if (IsValid(FoundDummy))
	{
		FoundDummy->SetDisplayName(DisplayName);
	}
}
