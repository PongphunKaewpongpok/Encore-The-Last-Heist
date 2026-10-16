// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Widgets/ItemOnGroundTab.h"
#include "Widgets/MainUI.h"

#include "CoreMinimal.h"
#include "Encore_TheLastHeist1PlayerController.h"
#include "ThiefPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ENCORE_THELASTHEIST1_API AThiefPlayerController : public AEncore_TheLastHeist1PlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	
public:
	virtual void Tick(float DeltaSeconds) override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UMainUI> MainUIWidgetClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UItemOnGroundTab> ItemOnGroundTabWidgetClass;
	
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UMainUI> MainUIWidget;
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UItemOnGroundTab> ItemOnGroundTabWidget;
	
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
	TObjectPtr<USoundBase> Sound_MusicBackground;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
	TObjectPtr<USoundBase> Sound_AmbienceBackground;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
	TObjectPtr<USoundBase> Sound_Chase;
	UPROPERTY()
	TObjectPtr<UAudioComponent> MusicBackgroundAudioComponent;
	UPROPERTY()
	TObjectPtr<UAudioComponent> AmbienceBackgroundAudioComponent;
	UPROPERTY()
	TObjectPtr<UAudioComponent> ChaseAudioComponent;

	bool bIsBeingChased = false;

	void SetChaseMusic(bool bStatus);
	
	UFUNCTION(Client, Reliable, BlueprintCallable, Category = "Audio")
	void Client_SetChaseMusic(bool bStatus);
};
