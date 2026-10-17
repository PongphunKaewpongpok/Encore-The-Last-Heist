// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/MainUI.h"

#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Encore_TheLastHeist1GameState.h"
#include "ThiefPlayerState.h"
#include "Components/VerticalBox.h"
#include "TimerManager.h"

FTimerHandle RefreshLeaderboardTimer;

void UMainUI::NativeConstruct()
{
	Super::NativeConstruct();
	
	GetWorld()->GetTimerManager().SetTimer(
		RefreshLeaderboardTimer,
		this,
		&UMainUI::BindPlayerStates,
		0.5f,
		false
	);
}

void UMainUI::BindPlayerStates()
{
	AEncore_TheLastHeist1GameState* GS = GetWorld()->GetGameState<AEncore_TheLastHeist1GameState>();
	if (!IsValid(GS)) return;

	for (APlayerState* PS : GS->PlayerArray)
	{
		AThiefPlayerState* ThiefPS = Cast<AThiefPlayerState>(PS);

		if (IsValid(ThiefPS))
		{
			ThiefPS->OnMoneyChanged.AddUniqueDynamic(
				this,
				&UMainUI::HandleMoneyChanged
			);
		}
	}
	
	RefreshLeaderboard();
}

void UMainUI::HandleMoneyChanged()
{
	RefreshLeaderboard();
}


void UMainUI::RefreshLeaderboard()
{
	
	if (!IsValid(LeaderboardList) || !IsValid(ThiefLeaderboardBarClass)) return;

	AEncore_TheLastHeist1GameState* GS = GetWorld()->GetGameState<AEncore_TheLastHeist1GameState>();
	if (!IsValid(GS)) return;

	LeaderboardList->ClearChildren();

	TArray<AThiefPlayerState*> Players = GS->GetLeaderboardPlayers();

	Players.Sort([](const AThiefPlayerState& A, const AThiefPlayerState& B)
	{
		return A.GetMoney() > B.GetMoney();
	});

	for (AThiefPlayerState* PS : Players)
	{
		if (!IsValid(PS))
		{
			continue;
		}

		UThiefLeaderboardBar* ThiefPlayerBar =
			CreateWidget<UThiefLeaderboardBar>(
				GetOwningPlayer(),
				ThiefLeaderboardBarClass
			);

		if (!IsValid(ThiefPlayerBar))
		{
			continue;
		}

		ThiefPlayerBar->Setup(PS);
		ThiefPlayerBar->SetPlayerName(PS->GetDisplayName());

		LeaderboardList->AddChild(ThiefPlayerBar);
	}
}




void UMainUI::SetWeight(const float Weight) const
{
	if (!WeightValueText) return;
	
	WeightValueText->SetText(FText::FromString(FString::SanitizeFloat(Weight) + TEXT(" kg")));
	
	float Alpha = FMath::Clamp(Weight / 70.f, 0.f, 1.f);

	FLinearColor Color = Alpha < 0.5f
		? FMath::Lerp(FLinearColor::Green, FLinearColor::Yellow, Alpha * 2.f)
		: FMath::Lerp(FLinearColor::Yellow, FLinearColor::Red, (Alpha - 0.5f) * 2.f);

	WeightValueText->SetColorAndOpacity(Color);
}

void UMainUI::SetMoney(const int32 Money) const
{
	if (!MoneyValueText) return;
	
	MoneyValueText->SetText(FText::AsNumber(Money));
}

void UMainUI::SetStorageMoney(const int32 StorageMoney) const
{
	if (!StorageMoneyValueText) return;
	
	StorageMoneyValueText->SetText(FText::AsNumber(StorageMoney));
}

void UMainUI::SetTimeLeft(const float RemainingTime)
{
	const int32 TotalSeconds = FMath::CeilToInt(RemainingTime);
	const int32 Minutes = TotalSeconds / 60;
	const int32 Seconds = TotalSeconds % 60;

	TimeLeftText->SetText(
		FText::FromString(
			FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds)
		)
	);
	
	if (Minutes == 0 && Seconds <= 30)
	{
		TimeLeftText->SetColorAndOpacity(FLinearColor::Red);
		
		if (LastTimeLeftSecond != TotalSeconds)
		{
			const float Progress = 1.0f - (TotalSeconds / 30.0f);
			const float Volume = FMath::Lerp(0.1f, 1.0f, FMath::Pow(Progress, 2.0f));

			UGameplayStatics::PlaySound2D(
				this,
				Sound_ClockTick,
				Volume
			);
		}
	}
	else
	{
		TimeLeftText->SetColorAndOpacity(FLinearColor::White);
	}
	
	LastTimeLeftSecond = TotalSeconds;
}
