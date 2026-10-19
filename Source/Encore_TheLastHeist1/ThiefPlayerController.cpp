// Fill out your copyright notice in the Description page of Project Settings.


#include "ThiefPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "Camera/CameraActor.h"

void AThiefPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (!IsLocalController()) return;
	
	
	if (MainUIWidgetClass)
	{
		MainUIWidget = CreateWidget<UMainUI>(
			this,
			MainUIWidgetClass
		);

		if (MainUIWidget)
		{
			MainUIWidget->AddToViewport();
		}
	}
	
	if (ItemOnGroundTabWidgetClass)
	{
		ItemOnGroundTabWidget = CreateWidget<UItemOnGroundTab>(
			this,
			ItemOnGroundTabWidgetClass
		);

		if (ItemOnGroundTabWidget)
		{
			ItemOnGroundTabWidget->AddToViewport();
		}
	}
	
	

	if (Sound_MusicBackground)
	{
		MusicBackgroundAudioComponent =
			UGameplayStatics::SpawnSound2D(
				this,
				Sound_MusicBackground,
				0.0f,
				1.0f,
				0.0f,
				nullptr,
				true,
				false
			);

		if (MusicBackgroundAudioComponent)
		{
			MusicBackgroundAudioComponent->SetVolumeMultiplier(0.15f);
		}
	}
	
	if (Sound_AmbienceBackground)
	{
		AmbienceBackgroundAudioComponent =
			UGameplayStatics::SpawnSound2D(
				this,
				Sound_AmbienceBackground,
				0.0f,
				1.0f,
				0.0f,
				nullptr,
				true,
				false
			);

		if (AmbienceBackgroundAudioComponent)
		{
			AmbienceBackgroundAudioComponent->SetVolumeMultiplier(0.1f);
		}
	}
}



void AThiefPlayerController::SetChaseMusic(bool bStatus)
{
	if (!IsLocalController()) return;
	if (bIsBeingChased == bStatus) return;
	
	bIsBeingChased = bStatus;
	if (bIsBeingChased)
	{
		if (Sound_Chase && !IsValid(ChaseAudioComponent))
		{
			ChaseAudioComponent =
				UGameplayStatics::SpawnSound2D(
					this,
					Sound_Chase,
					0.0f,
					1.0f,
					0.0f,
					nullptr,
					true,
					false
				);
		}
		else if (IsValid(ChaseAudioComponent) && !ChaseAudioComponent->IsPlaying())
		{
			ChaseAudioComponent->Play(0.0f);
		}
	}
}



void AThiefPlayerController::OnGameEnd()
{
	if (!IsLocalController()) return;
	if (bGameEnd) return;
	
	bGameEnd = true;
	if (!IsValid(MainUIWidget) || !IsValid(MainUIWidget->FullBlackScreenFadable)) return;

	MainUIWidget->FullBlackScreenFadable->OnFadeFinished.AddUniqueDynamic(
		this,
		&AThiefPlayerController::OnBlackScreenFadeFinished
	);
	
	
	
	MainUIWidget->FullBlackScreenFadable->FadeToBlackScreen();
}

void AThiefPlayerController::OnBlackScreenFadeFinished()
{
	UE_LOG(LogTemp, Warning, TEXT("Complete BC..."))
	MainUIWidget->SwitchToFullLeaderboard();
	
	
	TArray<AActor*> Cameras;
	UGameplayStatics::GetAllActorsWithTag(
		GetWorld(),
		FName("EndGameCamera"),
		Cameras
	);
	if (!IsValid(Cameras[0])) return;
	
	ACameraActor* TargetCamera = Cast<ACameraActor>(Cameras[0]);
	if (!IsValid(TargetCamera)) return;
	
	SetViewTargetWithBlend(TargetCamera, 0.0f);
	
	
	MainUIWidget->FullBlackScreenFadable->FadeToNormalScreen();
}




void AThiefPlayerController::Client_SetChaseMusic_Implementation(bool bStatus)
{
	SetChaseMusic(bStatus);
}

void AThiefPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!IsLocalController()) return;

	const float TargetBackgroundVolume = bIsBeingChased ? 0.0f : 0.15f;
	const float TargetChaseVolume = bIsBeingChased ? 0.15f : 0.0f;

	if (IsValid(MusicBackgroundAudioComponent))
	{
		const float CurrentVolume = MusicBackgroundAudioComponent->VolumeMultiplier;
		const float NewVolume = FMath::FInterpTo(
			CurrentVolume,
			TargetBackgroundVolume,
			DeltaSeconds,
			0.5f
		);
		MusicBackgroundAudioComponent->SetVolumeMultiplier(NewVolume);
	}

	if (IsValid(ChaseAudioComponent))
	{
		const float CurrentVolume = ChaseAudioComponent->VolumeMultiplier;
		const float NewVolume = FMath::FInterpTo(
			CurrentVolume,
			TargetChaseVolume,
			DeltaSeconds,
			0.5f
		);
		ChaseAudioComponent->SetVolumeMultiplier(NewVolume);

		if (!bIsBeingChased && NewVolume < 0.001f && ChaseAudioComponent->IsPlaying())
		{
			ChaseAudioComponent->Stop();
		}
	}
}
