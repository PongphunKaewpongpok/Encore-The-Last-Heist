// Fill out your copyright notice in the Description page of Project Settings.


#include "Encore_TheLastHeist1GameState.h"

#include "ThiefPlayerController.h"
#include "Net/UnrealNetwork.h"
#include "ThiefPlayerState.h"
#include "Encore_TheLastHeist1GameMode.h"
#include "Components/AudioComponent.h"

AEncore_TheLastHeist1GameState::AEncore_TheLastHeist1GameState()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

void AEncore_TheLastHeist1GameState::AddPlayerState(
	APlayerState* PlayerState)
{
	Super::AddPlayerState(PlayerState);

	if (AThiefPlayerState* ThiefPS =
		Cast<AThiefPlayerState>(PlayerState))
	{
		OnThiefPlayerStateAdded.Broadcast(ThiefPS);
	}
}

void AEncore_TheLastHeist1GameState::RemovePlayerState(
	APlayerState* PlayerState)
{
	Super::RemovePlayerState(PlayerState);
}

void AEncore_TheLastHeist1GameState::BeginPlay()
{
	Super::BeginPlay();
}

void AEncore_TheLastHeist1GameState::Multicast_ReadyForTravelToLobby_Implementation()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!IsValid(PC)) return;

	AThiefPlayerController* ThiefPC = Cast<AThiefPlayerController>(PC);
	if (!IsValid(ThiefPC)) return;
	
	ThiefPC->MusicEndGameAudioComponent->Stop();
}

void AEncore_TheLastHeist1GameState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!HasAuthority()) return;

	if (RemainingTime > 0.0f)
	{
		RemainingTime = FMath::Max(0.0f, RemainingTime - DeltaSeconds);
		OnRep_RemainingTime();
	}

	if (RemainingTime <= 0.0f && !bReturnCountdownStarted)
	{
		bReturnCountdownStarted = true;

		AEncore_TheLastHeist1GameMode* GM = GetWorld()->GetAuthGameMode<AEncore_TheLastHeist1GameMode>();

		if (IsValid(GM))
		{
			GM->StartCountdownReturnToLobby(10.0f);
		}
	}
}

void AEncore_TheLastHeist1GameState::OnRep_RemainingTime()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!IsValid(PC)) return;

	AThiefPlayerController* ThiefPC = Cast<AThiefPlayerController>(PC);
	if (!IsValid(ThiefPC)) return;
	
	ThiefPC->MainUIWidget->SetTimeLeft(RemainingTime);
	
	if (RemainingTime <= 0.0f)
	{
		ThiefPC->OnGameEnd();
	}
}

TArray<AThiefPlayerState*> AEncore_TheLastHeist1GameState::GetLeaderboardPlayers() const
{
	TArray<AThiefPlayerState*> Players;

	for (APlayerState* PS : PlayerArray)
	{
		if (AThiefPlayerState* ThiefPS = Cast<AThiefPlayerState>(PS))
		{
			Players.Add(ThiefPS);
		}
	}

	return Players;
}

void AEncore_TheLastHeist1GameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEncore_TheLastHeist1GameState, RemainingTime);
}
