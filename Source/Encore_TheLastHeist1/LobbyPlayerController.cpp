// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerController.h"

#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "Camera/CameraActor.h"


FTimerHandle DummyTimerHandle;


void ALobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (!IsLocalController()) return;
	
	

	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
	SetInputMode(FInputModeGameAndUI());
	
	TArray<AActor*> Cameras;
	UGameplayStatics::GetAllActorsWithTag(
		GetWorld(),
		FName("LobbyCamera"),
		Cameras
	);

	if (Cameras.IsEmpty()) return;

	LobbyCamera = Cast<ACameraActor>(Cameras[0]);
	if (!IsValid(LobbyCamera)) return;

	InitialRotation = LobbyCamera->GetActorRotation();
	SetViewTarget(LobbyCamera);

	
	
	UGameplayStatics::GetAllActorsWithTag(
		GetWorld(),
		FName("Dummy"),
		DummysArray
	);
	
	TArray<AActor*> RedSpotlightArray;
	UGameplayStatics::GetAllActorsWithTag(
		GetWorld(),
		FName("RedSpotlight"),
		RedSpotlightArray
	);
	RedSpotlight = Cast<USpotLightComponent>(RedSpotlightArray[0]->GetRootComponent());
	RedSpotlight->SetVisibility(false);
	
	GetWorld()->GetTimerManager().SetTimer(
		DummyTimerHandle,
		this,
		&ALobbyPlayerController::SwitchLight,
		5.0f,
		true
	);
	
	
	
	if (Sound_LobbyMusic)
	{
		LobbyMusicAudioComponent = UGameplayStatics::SpawnSound2D(
			this,
			Sound_LobbyMusic,
			0.5f,
			1.0f,
			0.0f,
			nullptr,
			true,
			false
		);
	}
}

void ALobbyPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsLocalController() || !IsValid(LobbyCamera))
	{
		return;
	}

	float MouseX = 0.0f;
	float MouseY = 0.0f;

	if (!GetMousePosition(MouseX, MouseY))
	{
		return;
	}

	int32 ViewportX = 0;
	int32 ViewportY = 0;
	GetViewportSize(ViewportX, ViewportY);

	if (ViewportX <= 0 || ViewportY <= 0)
	{
		return;
	}

	const float CenterX = ViewportX * 0.5f;
	const float CenterY = ViewportY * 0.5f;

	const float OffsetX = (MouseX - CenterX) / CenterX;
	const float OffsetY = (MouseY - CenterY) / CenterY;

	CurrentYaw = FMath::Clamp(
		OffsetX * MaxYaw,
		-MaxYaw,
		MaxYaw
	);

	CurrentPitch = FMath::Clamp(
		-OffsetY * MaxPitch,
		-MaxPitch,
		MaxPitch
	);

	FRotator NewRotation = InitialRotation;
	NewRotation.Yaw += CurrentYaw;
	NewRotation.Pitch += CurrentPitch;

	LobbyCamera->SetActorRotation(NewRotation);
}

void ALobbyPlayerController::SwitchLight()
{
	if (DummysArray.IsEmpty()) return;
	
	RedSpotlight->SetVisibility(!RedSpotlight->IsVisible());
	if (!RedSpotlight->IsVisible()) return;
	
	if (!DummysArray[0]->GetRootComponent()->IsVisible())
	{
		DummysArray[0]->GetRootComponent()->SetVisibility(true, true);
		DummysArray[1]->GetRootComponent()->SetVisibility(false, false);
	}
	else
	{
		DummysArray[0]->GetRootComponent()->SetVisibility(false, true);
		DummysArray[1]->GetRootComponent()->SetVisibility(true, false);
	}
}
