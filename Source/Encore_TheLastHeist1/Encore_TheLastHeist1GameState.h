// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Encore_TheLastHeist1GameState.generated.h"

/**
 * 
 */
UCLASS()
class ENCORE_THELASTHEIST1_API AEncore_TheLastHeist1GameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AEncore_TheLastHeist1GameState();
	virtual void Tick(float DeltaSeconds) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	
	UPROPERTY(ReplicatedUsing = OnRep_RemainingTime, BlueprintReadOnly)
	float RemainingTime = 35.0f;
	UFUNCTION()
	void OnRep_RemainingTime();
	
	UFUNCTION(BlueprintCallable, Category = "Leaderboard")
	TArray<AThiefPlayerState*> GetLeaderboardPlayers() const;

protected:
	virtual void BeginPlay() override;
};