// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameState.h"

#include "Net/UnrealNetwork.h"
#include "LobbyPlayerController.h"
#include "LobbyPlayerState.h"

ALobbyGameState::ALobbyGameState()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

void ALobbyGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ALobbyGameState, bIsStarting);
}



void ALobbyGameState::AddPlayerState(APlayerState* PlayerState)
{
	Super::AddPlayerState(PlayerState);

	if (ALobbyPlayerState* LobbyPS = Cast<ALobbyPlayerState>(PlayerState))
	{
		OnLobbyPlayerStateAdded.Broadcast(LobbyPS);
	}
}

void ALobbyGameState::RemovePlayerState(APlayerState* PlayerState)
{
	Super::RemovePlayerState(PlayerState);
}

TArray<ALobbyPlayerState*> ALobbyGameState::GetAllLobbyPlayerStates() const
{
	TArray<ALobbyPlayerState*> Players;

	for (APlayerState* PS : PlayerArray)
	{
		if (ALobbyPlayerState* LobbyPS = Cast<ALobbyPlayerState>(PS))
		{
			Players.Add(LobbyPS);
		}
	}

	return Players;
}


void ALobbyGameState::OnRep_IsStarting()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	ALobbyPlayerController* LobbyPC = Cast<ALobbyPlayerController>(PC);

	if (IsValid(LobbyPC))
	{
		LobbyPC->GameStarting();
	}
}
