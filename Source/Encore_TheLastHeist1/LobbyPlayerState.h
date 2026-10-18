// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "LobbyPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class ENCORE_THELASTHEIST1_API ALobbyPlayerState : public APlayerState
{
	GENERATED_BODY()
	
protected:
	virtual void CopyProperties(APlayerState* PlayerState) override;
	
public:
	virtual auto GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const -> void override;
	
	UPROPERTY(ReplicatedUsing = OnRep_DisplayName, BlueprintReadWrite, Category = "Player Data")
	FText DisplayName = FText::FromString("Anonymous");;
	UFUNCTION(BlueprintCallable, Category = "Player Data")
	void OnRep_DisplayName();
	
	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Player Data")
	bool IsReady = false;
	
	

};
