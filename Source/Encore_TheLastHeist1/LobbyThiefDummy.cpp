// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyThiefDummy.h"

#include "Widgets/LobbyDisplayName.h"
#include "Components/WidgetComponent.h"

// Sets default values
ALobbyThiefDummy::ALobbyThiefDummy()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetSkeletalMesh(ThiefSkeletalMesh);
	
	FloatingWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("FloatingWidget"));
	FloatingWidget->SetupAttachment(SkeletalMesh, TEXT("Head"));
	FloatingWidget->SetRelativeLocation(FVector(0.0f, -30.0f, 0.0f));
	FloatingWidget->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	FloatingWidget->SetWidgetSpace(EWidgetSpace::Screen);
	FloatingWidget->SetWidgetClass(LobbyDisplayNameWidgetClass);
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

void ALobbyThiefDummy::AddWidget(ULobbyDisplayName* TargetWidget)
{
	FloatingWidget->SetWidget(TargetWidget);
}
