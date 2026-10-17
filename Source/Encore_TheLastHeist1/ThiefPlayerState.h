// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ThiefPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class ENCORE_THELASTHEIST1_API AThiefPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	AThiefPlayerState();

protected:
	UPROPERTY(ReplicatedUsing = OnRep_Money, BlueprintReadOnly, Category = "Player Data")
	int32 Money = 0;
	UFUNCTION()
	void OnRep_Money();
	
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Player Data")
	FText DisplayName = FText::FromString("Anonymous");

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMoneyChanged);
	UPROPERTY(BlueprintAssignable, Category = "Player Data")
	FOnMoneyChanged OnMoneyChanged;
	
	void UpdateLeaderboard();
	
	UFUNCTION(BlueprintCallable, Category = "Player Data")
	int32 GetMoney() const { return Money; }
	UFUNCTION(BlueprintCallable, Category = "Player Data")
	void SetMoney(int32 NewMoney);
	
	UFUNCTION(BlueprintCallable, Category = "Player Data")
	FText GetDisplayName() const { return DisplayName; }
	UFUNCTION(BlueprintCallable, Category = "Player Data")
	void SetDisplayName(const FText& NewDisplayName);
};
