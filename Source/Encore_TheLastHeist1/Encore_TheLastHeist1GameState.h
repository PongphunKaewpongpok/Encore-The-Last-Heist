// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Encore_TheLastHeist1GameState.generated.h"

class AThiefPlayerState;
/**
 * 
 */
UCLASS()
class ENCORE_THELASTHEIST1_API AEncore_TheLastHeist1GameState : public AGameStateBase
{
	GENERATED_BODY()
	
	UPROPERTY()
	bool bReturnCountdownStarted = false;
public:
	AEncore_TheLastHeist1GameState();
	
	virtual void AddPlayerState(APlayerState* PlayerState) override;
	virtual void RemovePlayerState(APlayerState* PlayerState) override;

	DECLARE_MULTICAST_DELEGATE_OneParam(
		FOnThiefPlayerStateAdded,
		AThiefPlayerState*
	);
	FOnThiefPlayerStateAdded OnThiefPlayerStateAdded;
	
	
	virtual void Tick(float DeltaSeconds) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	
	UPROPERTY(ReplicatedUsing = OnRep_RemainingTime, BlueprintReadOnly)
	float RemainingTime = 180.0f;
	UFUNCTION()
	void OnRep_RemainingTime();
	
	UFUNCTION(BlueprintCallable, Category = "Leaderboard")
	TArray<AThiefPlayerState*> GetLeaderboardPlayers() const;

protected:
	virtual void BeginPlay() override;
	
public:
	void CheckPlayersIsAlive();
	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ReadyForTravelToLobby();
};