// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/ThiefLeaderboardBar.h"

#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "ThiefPlayerState.h"
#include "ThiefPlayerController.h"




void UThiefLeaderboardBar::Setup(AThiefPlayerState* InPlayerState)
{
	if (!IsValid(InPlayerState)) return;
	
	PlayerState = InPlayerState;
	
	AThiefPlayerController* PC = Cast<AThiefPlayerController>(GetOwningPlayer());
			
	if (IsValid(PC) && PC->GetPlayerState<AThiefPlayerState>() == PlayerState)
	{
		SetBarColor(FLinearColor(0.0f, 0.85f, 0.0f, 0.5f));
	}
	else
	{
		if (InPlayerState->GetIsAlive())
		{
			SetBarColor(FLinearColor(0.086f, 0.086f, 0.086f, 0.5f));
		}
		else
		{
			SetBarColor(FLinearColor(0.85f, 0.0f, 0.0f, 0.5f));
		}
	}
	
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

void UThiefLeaderboardBar::SetBarColor(const FLinearColor Color) const
{
	if (!PlayerBar) return;
	
	PlayerBar->SetBrushColor(Color);
}

void UThiefLeaderboardBar::SetShowDeathIcon(const bool bIsShow) const
{
	if (!DeathIcon) return;
	
	DeathIcon->SetVisibility(bIsShow ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}
