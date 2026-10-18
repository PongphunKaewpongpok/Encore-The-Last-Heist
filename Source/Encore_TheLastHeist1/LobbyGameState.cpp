// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameState.h"

#include "Net/UnrealNetwork.h"
#include "LobbyPlayerController.h"

void ALobbyGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ALobbyGameState, bIsStarting);
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
