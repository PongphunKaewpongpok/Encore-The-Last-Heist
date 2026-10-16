// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/ThiefLeaderboardBar.h"

#include "Components/TextBlock.h"
#include "ThiefPlayerState.h"




void UThiefLeaderboardBar::Setup(AThiefPlayerState* InPlayerState)
{
	PlayerState = InPlayerState;
	UpdateBar();
}

void UThiefLeaderboardBar::UpdateBar()
{
	if (!IsValid(PlayerState)) return;

	SetMoney(PlayerState->GetMoney());
}


void UThiefLeaderboardBar::SetPlayerName(const FText& PlayerName) const
{
	if (!PlayerNameValueText) return;
	
	PlayerNameValueText->SetText(PlayerName);
}

void UThiefLeaderboardBar::SetMoney(const int32 Money) const
{
	if (!MoneyValueText) return;
	
	MoneyValueText->SetText(FText::AsNumber(Money));
}
