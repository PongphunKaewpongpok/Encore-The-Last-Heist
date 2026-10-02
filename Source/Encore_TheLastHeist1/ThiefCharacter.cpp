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
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"





AThiefCharacter::AThiefCharacter()
{
	ThiefComponent = CreateDefaultSubobject<UThiefComponent>(TEXT("ThiefComponent"));

	KickReach = CreateDefaultSubobject<USphereComponent>(TEXT("KickReach"));
	KickReach->SetupAttachment(GetMesh());
	KickReach->SetSphereRadius(KickRange);
	KickReach->SetRelativeLocation(FVector(0,0,90));
	KickReach->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	KickReach->SetCollisionResponseToAllChannels(ECR_Ignore);
	KickReach->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	KickReach->SetGenerateOverlapEvents(true);
}

void AThiefCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Sprinting
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AThiefCharacter::ToggleSprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AThiefCharacter::ToggleSprint);
		
		// Kicking
		EnhancedInputComponent->BindAction(KickAction, ETriggerEvent::Started, this, &AThiefCharacter::DoKick);
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

void AThiefCharacter::UpdateSprint()
{
	if (!bDoAnimationDone) { return; }
	
	if (ThiefComponent->bIsSprint)
	{
		GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 100.0f;
	}
}


void AThiefCharacter::StartRolling()
{
	if (!bDoAnimationDone) { return; }

	FVector Direction = GetVelocity().GetSafeNormal2D();
	if (Direction.IsNearlyZero()) { return; }

	bDoAnimationDone = false;
	SetCanJump(false);

	SetActorRotation(Direction.Rotation());

	const float RollingSpeed = GetCharacterMovement()->MaxWalkSpeed + 100.0f;
	GetCharacterMovement()->MaxWalkSpeed = RollingSpeed;

	
	
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance || !RollingAnimation) { return; }

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(
		this,
		&AThiefCharacter::OnMontageFinished
	);

	AnimInstance->Montage_Play(RollingAnimation);
	AnimInstance->Montage_SetEndDelegate(
		EndDelegate,
		RollingAnimation
	);
}




void AThiefCharacter::Multicast_Kick_Implementation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance || !KickingAnimation) { return; }

	bDoAnimationDone = false;
	SetCanJump(false);
	GetCharacterMovement()->MaxWalkSpeed = 0;
	GetCharacterMovement()->StopMovementImmediately();
	
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(
		this,
		&AThiefCharacter::OnMontageFinished
	);

	AnimInstance->Montage_Play(KickingAnimation);
	AnimInstance->Montage_SetEndDelegate(
		EndDelegate,
		KickingAnimation
	);
}

void AThiefCharacter::Multicast_Knockdown_Implementation(const FVector& Direction, const float Strength)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance || !GotKnockdownAnimation) { return; }

	bDoAnimationDone = false;
	SetCanJump(false);
	GetCharacterMovement()->MaxWalkSpeed = 0;
	GetCharacterMovement()->StopMovementImmediately();
	Knockdown(Direction, Strength);
	
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(
		this,
		&AThiefCharacter::OnMontageFinished
	);

	AnimInstance->Montage_Play(GotKnockdownAnimation);
	AnimInstance->Montage_SetEndDelegate(
		EndDelegate,
		GotKnockdownAnimation
	);
}

void AThiefCharacter::Knockdown(const FVector& Direction, const float Strength)
{
	const FVector KnockbackVelocity = Direction.GetSafeNormal() * Strength;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance || !GotKnockdownAnimation) { return; }
	
	LaunchCharacter(
		KnockbackVelocity,
		true,
		true
	);
}

void AThiefCharacter::OnMontageFinished(UAnimMontage* Montage, bool bInterrupted)
{
	bDoAnimationDone = true;
	SetCanJump(true);

	UpdateSprint();
}


void AThiefCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	
	StartRolling();
}

void AThiefCharacter::DoMove(float Right, float Forward)
{
	Super::DoMove(Right, Forward);
	
	// SHOULD Binding Notify in future
	if (ThiefComponent->bIsSprint)
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
	if (!bDoAnimationDone) { return; }
	if (GetCharacterMovement()->IsFalling()) { return; }
	
	const float Now = GetWorld()->GetTimeSeconds();
	if (Now - LastKickTime < KickCooldown) { return; }
	LastKickTime = Now;
	
	Multicast_Kick();
	
	TArray<AActor*> Overlapping;
	KickReach->GetOverlappingActors(Overlapping, AThiefCharacter::StaticClass());
		
	TSet<AActor*> AlreadyHit;
	const FVector Origin  = GetActorLocation();
	const FVector Forward = GetActorForwardVector();
	const float CosThresh = FMath::Cos(
	FMath::DegreesToRadians(KickHalfAngle));
		
	for (AActor* T : Overlapping)
	{
		if (!T || T == this) continue;
		if (AlreadyHit.Contains(T)) continue;
			
		const FVector To = T->GetActorLocation() - Origin;
		const float Dist = To.Size();
			
		if (Dist > KickRange) continue;
			
		const FVector Dir = To.GetSafeNormal();
		
		if (FVector::DotProduct(Forward, Dir) < CosThresh) continue;
		
		if (AThiefCharacter* Target = Cast<AThiefCharacter>(T))
		{
			Target->Multicast_Knockdown(Forward, KickStrength);
			
			AlreadyHit.Add(T);
		}
	}
}

bool AThiefCharacter::ServerKick_Validate()
{
	return true;
}

void AThiefCharacter::ServerSprint_Implementation(bool bIsSprint)
{
	ThiefComponent->bIsSprint = bIsSprint;
	UpdateSprint();
}

bool AThiefCharacter::ServerSprint_Validate(bool bIsSprint)
{
	return true;
}

