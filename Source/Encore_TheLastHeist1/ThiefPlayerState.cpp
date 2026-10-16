// Fill out your copyright notice in the Description page of Project Settings.


#include "ThiefPlayerState.h"

#include "Net/UnrealNetwork.h"

AThiefPlayerState::AThiefPlayerState()
{
	bReplicates = true;
}

void AThiefPlayerState::SetMoney(int32 NewMoney)
{
	if (!HasAuthority()) return;

	if (Money == NewMoney) return;
	Money = NewMoney;

	OnMoneyChanged.Broadcast();
	ForceNetUpdate();
}

void AThiefPlayerState::SetDisplayName(const FText& NewDisplayName)
{
	if (!HasAuthority()) return;
	if (DisplayName.EqualTo(NewDisplayName)) return;

	DisplayName = NewDisplayName;
}

void AThiefPlayerState::OnRep_Money()
{
	OnMoneyChanged.Broadcast();
}

void AThiefPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AThiefPlayerState, Money);
	DOREPLIFETIME(AThiefPlayerState, DisplayName);
}
