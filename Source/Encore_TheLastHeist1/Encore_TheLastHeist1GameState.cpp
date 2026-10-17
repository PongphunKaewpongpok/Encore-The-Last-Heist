// Fill out your copyright notice in the Description page of Project Settings.


#include "Encore_TheLastHeist1GameState.h"

#include "ThiefPlayerController.h"
#include "Net/UnrealNetwork.h"
#include "ThiefPlayerState.h"

AEncore_TheLastHeist1GameState::AEncore_TheLastHeist1GameState()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

void AEncore_TheLastHeist1GameState::BeginPlay()
{
	Super::BeginPlay();
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
}

void AEncore_TheLastHeist1GameState::OnRep_RemainingTime()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!IsValid(PC)) return;

	AThiefPlayerController* ThiefPC = Cast<AThiefPlayerController>(PC);
	if (!IsValid(ThiefPC)) return;
	
	ThiefPC->MainUIWidget->SetTimeLeft(RemainingTime);
}

TArray<AThiefPlayerState*> AEncore_TheLastHeist1GameState::GetLeaderboardPlayers() const
{
	TArray<AThiefPlayerState*> Players;

	UE_LOG(LogTemp, Warning, TEXT("%d"), PlayerArray.Num());
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
