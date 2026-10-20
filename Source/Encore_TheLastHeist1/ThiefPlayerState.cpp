// Fill out your copyright notice in the Description page of Project Settings.


#include "ThiefPlayerState.h"

#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "LobbyPlayerState.h"

AThiefPlayerState::AThiefPlayerState()
{
	bReplicates = true;
}

void AThiefPlayerState::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);
	
	ALobbyPlayerState* NewPlayerState = Cast<ALobbyPlayerState>(PlayerState);

	if (IsValid(NewPlayerState))
	{
		if (DisplayName.ToString().Equals(TEXT("Anonymous"))) return;

		NewPlayerState->DisplayName = DisplayName;
	}
}

void AThiefPlayerState::SetMoney(int32 NewMoney)
{
	if (!HasAuthority()) return;

	if (Money == NewMoney) return;
	Money = NewMoney;

	OnDataChanged.Broadcast();
	ForceNetUpdate();
}

void AThiefPlayerState::SetIsAlive(const bool InIsAlive)
{
	if (!HasAuthority()) return;

	if (bIsAlive == InIsAlive) return;
	bIsAlive = InIsAlive;

	if (!InIsAlive)
	{
		PlaySomeoneDeathSound();
	}
	OnDataChanged.Broadcast();
	ForceNetUpdate();
}

void AThiefPlayerState::SetDisplayName(const FText& NewDisplayName)
{
	if (!HasAuthority()) return;
	if (DisplayName.EqualTo(NewDisplayName)) return;

	DisplayName = NewDisplayName;
	
	OnDataChanged.Broadcast();
	ForceNetUpdate();
}

void AThiefPlayerState::PlaySomeoneDeathSound() const
{
	if (IsValid(Sound_PlayerDeath_1))
	{
		UGameplayStatics::PlaySound2D(this, Sound_PlayerDeath_1);
	}

	if (IsValid(Sound_PlayerDeath_2))
	{
		UGameplayStatics::PlaySound2D(this, Sound_PlayerDeath_2);
	}
	
	
}

void AThiefPlayerState::OnRep_Money()
{
	OnDataChanged.Broadcast();
}

void AThiefPlayerState::OnRep_IsAlive()
{
	if (!GetIsAlive())
	{
		PlaySomeoneDeathSound();
	}
	
	OnDataChanged.Broadcast();
}

void AThiefPlayerState::OnRep_DisplayName()
{
	OnDataChanged.Broadcast();
}

void AThiefPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AThiefPlayerState, Money);
	DOREPLIFETIME(AThiefPlayerState, bIsAlive);
	DOREPLIFETIME(AThiefPlayerState, DisplayName);
}
