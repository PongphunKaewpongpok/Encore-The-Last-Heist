// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ThiefLeaderboardBar.generated.h"

class UTextBlock;
class UBorder;
class UImage;
/**
 * 
 */
UCLASS()
class ENCORE_THELASTHEIST1_API UThiefLeaderboardBar : public UUserWidget
{
	GENERATED_BODY()
	
	
public:
	UPROPERTY(BlueprintReadOnly, Category = "Leaderboard")
	TObjectPtr<AThiefPlayerState> PlayerState;
	UFUNCTION(BlueprintCallable, Category = "Leaderboard")
	void Setup(AThiefPlayerState* InPlayerState);
	UFUNCTION(BlueprintCallable, Category = "Leaderboard")
	void UpdateBar();
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<UBorder> PlayerBar;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<UTextBlock> PlayerNameValueText;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<UTextBlock> MoneyValueText;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<UImage> RankingIcon;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<UImage> DeathIcon;
	
	void SetPlayerName(const FText& PlayerName) const;
	void SetMoney(const int32 Money) const;
	void SetBarColor(const FLinearColor Color) const;
	void SetShowDeathIcon(const bool bIsShow) const;
	void SetShowRankIcon(const bool bIsShow) const;
};
