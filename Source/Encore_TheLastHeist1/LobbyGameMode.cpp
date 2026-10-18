// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"

void ALobbyGameMode::BeginPlay()
{
	Super::BeginPlay();
}

ALobbyGameMode::ALobbyGameMode()
{
	bUseSeamlessTravel = true;
}

void ALobbyGameMode::TravelToInGame()
{
	GetWorld()->ServerTravel(TEXT("/Game/Levels/L_InGame_AbandonedHouse?listen"));
}
