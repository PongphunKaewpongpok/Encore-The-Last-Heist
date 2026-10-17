// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SpotLightComponent.h"

#include "CoreMinimal.h"
#include "Encore_TheLastHeist1PlayerController.h"
#include "LobbyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ENCORE_THELASTHEIST1_API ALobbyPlayerController : public AEncore_TheLastHeist1PlayerController
{
	GENERATED_BODY()
	
	UPROPERTY()
	TObjectPtr<ACameraActor> LobbyCamera;
	
	UPROPERTY(EditAnywhere, Category = "Lobby Camera")
	float MouseSensitivity = 0.1f;

	UPROPERTY(EditAnywhere, Category = "Lobby Camera")
	float MaxYaw = 5.0f;

	UPROPERTY(EditAnywhere, Category = "Lobby Camera")
	float MaxPitch = 5.0f;

	FRotator InitialRotation;

	float CurrentYaw = 0.0f;
	float CurrentPitch = 0.0f;
	
protected:
	virtual void BeginPlay() override;
	
public:
	virtual void Tick(float DeltaSeconds) override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
	TObjectPtr<USoundBase> Sound_LobbyMusic;
	UPROPERTY()
	TObjectPtr<UAudioComponent> LobbyMusicAudioComponent;
	
	UPROPERTY()
	TArray<AActor*> DummysArray;
	UPROPERTY()
	USpotLightComponent* RedSpotlight;
	
	void SwitchLight();
};
