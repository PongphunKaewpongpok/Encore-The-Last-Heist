// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyThiefDummy.h"

#include "LobbyPlayerState.h"

// Sets default values
ALobbyThiefDummy::ALobbyThiefDummy()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALobbyThiefDummy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALobbyThiefDummy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ALobbyThiefDummy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ALobbyThiefDummy::SetDisplayName_Implementation(const FText& DisplayName)
{
}

