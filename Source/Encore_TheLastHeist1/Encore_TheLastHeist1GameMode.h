// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ItemDataAsset.h"
#include "Engine/TriggerBox.h"
#include "InteractableActor.h"

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Encore_TheLastHeist1GameMode.generated.h"

/**
 *  Simple GameMode for a third person game
 */
UCLASS(abstract)
class AEncore_TheLastHeist1GameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
	FTimerHandle ReturnToLobbyTimerHandle;
public:
	/** Constructor */
	AEncore_TheLastHeist1GameMode();
	void StartCountdownReturnToLobby(const float Duration);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TObjectPtr<UItemDataAsset>> SpawnableItems;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AInteractableActor> InteractableClass;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	TArray<TObjectPtr<ATriggerBox>> SpawnableArea;
	
	
	void SpawnRandomItems(const int SpawnCount);
	void SpawnItem(const FVector& Location, UItemDataAsset* Item);
	
	void TravelToLobby();
};




