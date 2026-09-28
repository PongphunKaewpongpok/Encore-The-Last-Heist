// Fill out your copyright notice in the Description page of Project Settings.


#include "ThiefCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Encore_TheLastHeist1.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AISense_Damage.h"
#include "Perception/AISense_Sight.h"

void AThiefCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Sprinting
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AThiefCharacter::ToggleSprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AThiefCharacter::ToggleSprint);
		
	}
	else
	{
		UE_LOG(LogEncore_TheLastHeist1, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


void AThiefCharacter::ToggleSprint(const FInputActionValue& Value)
{
	ServerSprint(Value.Get<bool>());
}

void AThiefCharacter::DoKick(const FInputActionValue& Value)
{
	ServerKick();
}

void AThiefCharacter::DoMove(float Right, float Forward)
{
	Super::DoMove(Right, Forward);
	
	// SHOULD Binding Notify in future
	if (GetCharacterMovement()->MaxWalkSpeed == 500.0f)
	{
		UAISense_Hearing::ReportNoiseEvent( GetWorld(),
											GetActorLocation(), 
											10.0f, 
											this, 
											SprintNoiseRange,
											TEXT("Footsteps"));
	}
}

void AThiefCharacter::ServerKick_Implementation()
{
	
}

bool AThiefCharacter::ServerKick_Validate()
{
	return true;
}

void AThiefCharacter::ServerSprint_Implementation(bool bIsSprint)
{
	if (bIsSprint)
	{
		GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 100.0f;
	}
}

bool AThiefCharacter::ServerSprint_Validate(bool bIsSprint)
{
	return true;
}
