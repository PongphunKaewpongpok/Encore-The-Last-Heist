// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "LobbyGameState.generated.h"

class ALobbyPlayerState;
/**
 * 
 */
UCLASS()
class ENCORE_THELASTHEIST1_API ALobbyGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	ALobbyGameState();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual void AddPlayerState(APlayerState* PlayerState) override;
	virtual void RemovePlayerState(APlayerState* PlayerState) override;
	
	TArray<ALobbyPlayerState*> GetAllLobbyPlayerStates() const;


	DECLARE_MULTICAST_DELEGATE_OneParam(
		FOnLobbyPlayerStateAdded,
		ALobbyPlayerState*
	);
	FOnLobbyPlayerStateAdded OnLobbyPlayerStateAdded;
	
	UPROPERTY(ReplicatedUsing = OnRep_IsStarting, BlueprintReadOnly)
	bool bIsStarting = false;
	UFUNCTION()
	void OnRep_IsStarting();
};
