// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "Components/VerticalBox.h"
#include "Widgets/ThiefLeaderboardBar.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainUI.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class ENCORE_THELASTHEIST1_API UMainUI : public UUserWidget
{
	GENERATED_BODY()
	
	int32 LastTimeLeftSecond = -1;
	
protected:
	virtual void NativeConstruct() override;
	
public:
	UFUNCTION()
	void HandleMoneyChanged();

	void BindSinglePlayerState(AThiefPlayerState* ThiefPS);

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<UVerticalBox> LeaderboardList;
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UThiefLeaderboardBar> ThiefLeaderboardBarClass;
	UFUNCTION(BlueprintCallable, Category = "UI")
	void RefreshLeaderboard();


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
	TObjectPtr<USoundBase> Sound_ClockTick;
	
	
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<UTextBlock> TimeLeftText;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<UTextBlock> WeightValueText;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<UTextBlock> MoneyValueText;
	UPROPERTY( BlueprintReadOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<UTextBlock> StorageMoneyValueText;
	
	void SetWeight(const float Weight) const;
	void SetMoney(const int32 Money) const;
	void SetStorageMoney(const int32 BankMoney) const;
	
	void SetTimeLeft(const float RemainingTime);
	
	
};
