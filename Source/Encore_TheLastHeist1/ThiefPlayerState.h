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
	
	UPROPERTY(ReplicatedUsing = OnRep_IsAlive, BlueprintReadOnly, Category = "Player Data")
	bool bIsAlive = true;
	UFUNCTION()
	void OnRep_IsAlive();
	
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Player Data")
	FText DisplayName = FText::FromString("Anonymous");
	
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
	TObjectPtr<USoundBase> Sound_PlayerDeath_1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
	TObjectPtr<USoundBase> Sound_PlayerDeath_2;

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDataChanged);
	UPROPERTY(BlueprintAssignable, Category = "Player Data")
	FOnDataChanged OnDataChanged;
	
	
	UFUNCTION(BlueprintCallable, Category = "Player Data")
	int32 GetMoney() const { return Money; }
	UFUNCTION(BlueprintCallable, Category = "Player Data")
	void SetMoney(int32 NewMoney);
	
	UFUNCTION(BlueprintCallable, Category = "Player Data")
	bool GetIsAlive() const { return bIsAlive; }
	UFUNCTION(BlueprintCallable, Category = "Player Data")
	void SetIsAlive(const bool InIsAlive);

	UFUNCTION(BlueprintCallable, Category = "Player Data")
	FText GetDisplayName() const { return DisplayName; }
	UFUNCTION(BlueprintCallable, Category = "Player Data")
	void SetDisplayName(const FText& NewDisplayName);
	
	void PlaySomeoneDeathSound() const;
};
