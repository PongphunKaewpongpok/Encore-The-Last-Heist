// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerController.h"

#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "Camera/CameraActor.h"
#include "LobbyPlayerState.h"
#include "LobbyGameMode.h"
#include "LobbyTextRenderActor.h"
#include "LobbyGameState.h"
#include "Widgets/LobbyDisplayName.h"
#include "LobbyThiefDummy.h"
#include "GameFramework/GameUserSettings.h"


FTimerHandle DummyTimerHandle;


void ALobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	// Graphic Settings
	if (UGameUserSettings* Settings = UGameUserSettings::GetGameUserSettings())
	{
		Settings->SetOverallScalabilityLevel(0);
		Settings->ApplySettings(false);
	}

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
	
	
	if (LobbyMainUIWidgetClass)
	{
		LobbyMainUIWidget = CreateWidget<ULobbyMainUI>(
			this,
			LobbyMainUIWidgetClass
		);

		if (LobbyMainUIWidget)
		{
			LobbyMainUIWidget->AddToViewport();
		}
	}
	
	if (LobbyDisplayNameWidgetClass)
	{
		LobbyDisplayNameWidget = CreateWidget<ULobbyDisplayName>(
			this,
			LobbyDisplayNameWidgetClass
		);
	}
	
	ALobbyThiefDummy* FoundDummy = nullptr;

	if (TActorIterator<ALobbyThiefDummy> It(GetWorld()); It)
	{
		FoundDummy = *It;
	}

	if (IsValid(FoundDummy))
	{
		if (LobbyDisplayNameWidget)
		{
			FoundDummy->AddWidget(LobbyDisplayNameWidget);
		}
	}
	
	
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

void ALobbyPlayerController::OnLeftClick()
{
	FHitResult Hit;

	if (GetHitResultUnderCursor(
		ECC_Visibility,
		false,
		Hit))
	{
		AActor* HitActor = Hit.GetActor();

		if (IsValid(HitActor))
		{
			ALobbyTextRenderActor* TargetTextRender = Cast<ALobbyTextRenderActor>(HitActor);
			if (!TargetTextRender) return;
			
			if (HasAuthority())
			{
				ServerStart();
			}
			else
			{
				ServerReady();
			}
		}
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
	if (bIsTraveling) return;
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

void ALobbyPlayerController::GameStarting()
{
	if (!IsLocalController()) return;
	
	GetWorldTimerManager().ClearTimer(DummyTimerHandle);
	bIsTraveling = true;
	
	LobbyMusicAudioComponent->Stop();
	
	bShowMouseCursor = false;
	bEnableClickEvents = false;
	bEnableMouseOverEvents = false;
	SetInputMode(FInputModeGameOnly());
}

void ALobbyPlayerController::ServerStart_Implementation()
{
	if (!HasAuthority()) return;
	
	ALobbyGameMode* GM = Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode());
	if (!IsValid(GM)) return;

	ALobbyPlayerState* PS = GetPlayerState<ALobbyPlayerState>();
	PS->IsReady = true;
	
	ALobbyGameState* GS = GetWorld()->GetGameState<ALobbyGameState>();
	GS->bIsStarting = true;
    GS->ForceNetUpdate();
	
	GameStarting();
	
	GM->TravelToInGame();
}

bool ALobbyPlayerController::ServerStart_Validate()
{
	return true;
}

void ALobbyPlayerController::ServerReady_Implementation()
{
	ALobbyPlayerState* PS = GetPlayerState<ALobbyPlayerState>();
	PS->IsReady = true;
}

bool ALobbyPlayerController::ServerReady_Validate()
{
	return true;
}

void ALobbyPlayerController::ServerSetDisplayName_Implementation(const FText& DisplayName)
{
	ALobbyPlayerState* PS = GetPlayerState<ALobbyPlayerState>();
	if (!IsValid(PS)) return;
	
	FText TrimmedText = FText::TrimPrecedingAndTrailing(DisplayName);
	FString InputString = TrimmedText.ToString();
	FString LeftString = InputString.Left(MaxDisplayNameLength);
	FText ResultText = FText::FromString(LeftString);
	
	PS->DisplayName = (ResultText.EqualTo(FText::FromString("")) ? FText::FromString("Anonymous") : ResultText);
	PS->OnRep_DisplayName();
}
