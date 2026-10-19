// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SpotLightComponent.h"
#include "Widgets/LobbyMainUI.h"


#include "CoreMinimal.h"
#include "Encore_TheLastHeist1PlayerController.h"
#include "LobbyPlayerController.generated.h"

class UInputAction;
class ULobbyDisplayName;
/**
 * 
 */
UCLASS()
class ENCORE_THELASTHEIST1_API ALobbyPlayerController : public AEncore_TheLastHeist1PlayerController
{
	GENERATED_BODY()
	
	int32 MaxDisplayNameLength = 16;
	
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
	
	
	bool bIsTraveling = false;
	
protected:
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable)
	void OnLeftClick();
public:
	virtual void Tick(float DeltaSeconds) override;
	
	UPROPERTY(BlueprintReadWrite, Category = "UI")
	TObjectPtr<ULobbyMainUI> LobbyMainUIWidget;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<ULobbyMainUI> LobbyMainUIWidgetClass;
	
	UPROPERTY(BlueprintReadWrite, Category = "UI")
	TObjectPtr<ULobbyDisplayName> LobbyDisplayNameWidget;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<ULobbyDisplayName> LobbyDisplayNameWidgetClass;
	
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
	TObjectPtr<USoundBase> Sound_LobbyMusic;
	UPROPERTY()
	TObjectPtr<UAudioComponent> LobbyMusicAudioComponent;
	
	UPROPERTY()
	TArray<AActor*> DummysArray;
	UPROPERTY()
	USpotLightComponent* RedSpotlight;
	
	void SwitchLight();
	void GameStarting();
	
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerSetDisplayName(const FText& DisplayName);
	
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerReady();
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerStart();
};
