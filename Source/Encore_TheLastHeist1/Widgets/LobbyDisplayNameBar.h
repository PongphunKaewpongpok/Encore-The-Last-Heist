// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyDisplayNameBar.generated.h"

class UBorder;
class UTextBlock;
class ALobbyPlayerState;
/**
 * 
 */
UCLASS()
class ENCORE_THELASTHEIST1_API ULobbyDisplayNameBar : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, Category = "Leaderboard")
	TObjectPtr<ALobbyPlayerState> PlayerState;
	UFUNCTION(BlueprintCallable, Category = "Leaderboard")
	void Setup(ALobbyPlayerState* InPlayerState);
	UFUNCTION(BlueprintCallable, Category = "Leaderboard")
	void UpdateBar();
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<UBorder> PlayerBar;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<UTextBlock> PlayerNameValueText;
	
	void SetPlayerName(const FText& PlayerName) const;
	void SetBarColor(const FLinearColor Color) const;
};
