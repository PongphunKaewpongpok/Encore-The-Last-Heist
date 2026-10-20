// Fill out your copyright notice in the Description page of Project Settings.


#include "ThiefCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Encore_TheLastHeist1.h"
#include "Perception/AISense_Hearing.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ThiefPlayerController.h"
#include "ThiefPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Widgets/PlayerHeadNametag.h"
#include "WeretigerCharacter.h"
#include "Components/CapsuleComponent.h"


AThiefCharacter::AThiefCharacter()
{
	ThiefComponent = CreateDefaultSubobject<UThiefComponent>(TEXT("ThiefComponent"));
	ThiefComponent->SetIsReplicated(true);
	
	KickReach = CreateDefaultSubobject<USphereComponent>(TEXT("KickReach"));
	KickReach->SetupAttachment(GetMesh());
	KickReach->SetSphereRadius(KickRange);
	KickReach->SetRelativeLocation(FVector(0,0,90));
	KickReach->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	KickReach->SetCollisionResponseToAllChannels(ECR_Ignore);
	KickReach->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	KickReach->SetGenerateOverlapEvents(true);
	
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	
	NameWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("NametagWidgetComponent"));
	NameWidgetComponent->SetupAttachment(GetMesh(), TEXT("Head"));
	NameWidgetComponent->SetRelativeLocation(FVector(0.0f, -30.0f, 0.0f));
	NameWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	NameWidgetComponent->SetDrawSize(FVector2D(200.f, 50.f));
	NameWidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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
	
		// Mouse Wheel
		EnhancedInputComponent->BindAction(MouseWheelAction, ETriggerEvent::Triggered, this, &AThiefCharacter::OnMouseWheel);
		
		// Interact
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AThiefCharacter::DoInteract);
	}
	else
	{
		UE_LOG(LogEncore_TheLastHeist1, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AThiefCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AThiefCharacter, ThiefComponent);
}

void AThiefCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	UpdateSprint();
	UpdateNameVisibility();
}

void AThiefCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (NameWidgetComponent && PlayerNameWidgetClass)
	{
		NameWidgetComponent->SetWidgetClass(
			PlayerNameWidgetClass
		);

		NameWidgetComponent->InitWidget();
	}

	UpdatePlayerNameWidget();
}

void AThiefCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	UpdatePlayerNameWidget();
}

void AThiefCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	UpdatePlayerNameWidget();
}


void AThiefCharacter::ToggleSprint(const FInputActionValue& Value)
{
	if (!IsValid(ThiefComponent)) return;
	if (!ThiefComponent->bIsAlive) return;
	
	const bool bSprint = Value.Get<bool>();

	ThiefComponent->bIsSprint = bSprint;

	ServerSprint(bSprint);
}

void AThiefCharacter::DoKick(const FInputActionValue& Value)
{
	if (!IsValid(ThiefComponent)) return;
	if (!ThiefComponent->bIsAlive) return;
	
	ServerKick();
}

void AThiefCharacter::OnMouseWheel(const FInputActionValue& Value)
{
	if (!IsLocallyControlled()) return;
	if (!IsValid(ThiefComponent)) return;
	if (!ThiefComponent->bIsAlive) return;
	
	const float WheelValue = Value.Get<float>();
	AThiefPlayerController* TargetPC = Cast<AThiefPlayerController>(GetController());
	
	if (WheelValue > 0.0f)
	{
		// Mouse Wheel Up
		TargetPC->ItemOnGroundTabWidget->ItemBarScrollingUp();
	}
	else if (WheelValue < 0.0f)
	{
		// Mouse Wheel Down
		TargetPC->ItemOnGroundTabWidget->ItemBarScrollingDown();
	}
}

void AThiefCharacter::DoInteract(const FInputActionValue& Value)
{
	if (!IsValid(ThiefComponent)) return;
	if (!ThiefComponent->bIsAlive) return;
	
	AThiefPlayerController* TargetPC = Cast<AThiefPlayerController>(GetController());
	if (!IsValid(TargetPC)) return;
	
	AInteractableActor* TargetItem = TargetPC->ItemOnGroundTabWidget->GetSelectedInteractableActor();
	if (!IsValid(TargetItem)) return;
	
	TargetPC->ItemOnGroundTabWidget->CollectItem();
	ServerInteract(TargetItem);
}




void AThiefCharacter::UpdateSprint()
{
	if (!bDoAnimationDone) return;
	if (!IsValid(ThiefComponent)) return;
	if (!ThiefComponent->bIsAlive) return;
	
	if (ThiefComponent->bIsSprint)
	{
		GetCharacterMovement()->MaxWalkSpeed = RunningSpeed - (RunningSpeed * FMath::Min(FMath::Max(1, ThiefComponent->GetWeight()), 70)/100);
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkingSpeed - (WalkingSpeed * FMath::Min(FMath::Max(1, ThiefComponent->GetWeight()), 50)/100);
	}
}


void AThiefCharacter::StartRolling()
{
	if (!bDoAnimationDone) return;
	if (!IsValid(ThiefComponent)) return;
	if (!ThiefComponent->bIsAlive) return;

	FVector Direction = GetVelocity().GetSafeNormal2D();
	if (Direction.IsNearlyZero()) return;

	bDoAnimationDone = false;
	SetCanJump(false);

	SetActorRotation(Direction.Rotation());

	const float RollingSpeed = GetCharacterMovement()->MaxWalkSpeed + 50.0f;
	GetCharacterMovement()->MaxWalkSpeed = RollingSpeed;

	
	
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance || !RollingAnimation) return;

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
	if (!AnimInstance || !KickingAnimation) return;

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
	if (!AnimInstance || !GotKnockdownAnimation) return;

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
	if (!AnimInstance || !GotKnockdownAnimation) return;
	
	LaunchCharacter(
		KnockbackVelocity,
		true,
		true
	);
}

void AThiefCharacter::OnMontageFinished(UAnimMontage* Montage, bool bInterrupted)
{
	if (!IsValid(ThiefComponent)) return;
	if (!ThiefComponent->bIsAlive) return;
	if (bInterrupted) return;
	
	bDoAnimationDone = true;
	SetCanJump(true);
}


void AThiefCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	
	StartRolling();
}

void AThiefCharacter::ServerInteract_Implementation(AInteractableActor* TargetItem)
{
	if (!TargetItem) return;
	if (!TargetItem->IsCanCollect(this)) return;
	if (!IsValid(ThiefComponent)) return;
	if (!ThiefComponent->bIsAlive) return;
	
	ThiefComponent->AddInventory(TargetItem->ItemData);
	
	TargetItem->Interact(this);
}

bool AThiefCharacter::ServerInteract_Validate(AInteractableActor* TargetItem)
{
	return true;
}

void AThiefCharacter::Multicast_RemoveOnGroundItem_Implementation(AInteractableActor* InteractableActor)
{
	InteractableActor->OnEndOverlapCollectable(
		nullptr,
		this,
		nullptr,
		0
	);
}

void AThiefCharacter::Multicast_GotExecuted_Implementation()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance || !DeathAnimation) return;

	AnimInstance->Montage_Play(DeathAnimation);
}

void AThiefCharacter::DropItem(const int32 Index)
{
	if (!IsValid(ThiefComponent)) return;
	if (!ThiefComponent->bIsAlive) return;
	
	ThiefComponent->RemoveInventoryAt(Index);
}

void AThiefCharacter::GotExecuted(AWeretigerCharacter* Weretiger)
{
	if (!IsValid(ThiefComponent)) return;
	if (!ThiefComponent->bIsAlive) return;
	if (!Weretiger) return;
	
	Multicast_GotExecuted();
	
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!IsValid(PC)) return;
	
	AThiefPlayerState* PS = GetPlayerState<AThiefPlayerState>();
	if (!IsValid(PS)) return;
	
	PS->SetIsAlive(false);
	ThiefComponent->bIsAlive = false;
	
	
	GetCharacterMovement()->StopMovementImmediately();
	SetCanJump(false);
	PC->SetIgnoreMoveInput(true);
	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->GravityScale = 0.0f;
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	
	
	FVector Start = GetActorLocation();
	FVector TargetLocation = Weretiger->GetActorLocation();

	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(
		Start,
		TargetLocation
	);

	SetActorRotation(FRotator(
		0.f,
		LookAtRotation.Yaw,
		0.f
	));
	
	
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance) return;
	
	if (DeathAnimation)
	{
		AnimInstance->Montage_Play(DeathAnimation);
	}
}

bool AThiefCharacter::IsDead() const
{
	if (!IsValid(ThiefComponent)) return false;
	
	return !ThiefComponent->bIsAlive;
}

void AThiefCharacter::DisableAllInputs()
{
	AThiefPlayerController* PC = Cast<AThiefPlayerController>(GetController());
	DisableInput(PC);
}

void AThiefCharacter::PlayFootstep()
{
	if (!GetWorld()) return;

	const FVector Start = GetActorLocation();
	const FVector End = Start - FVector(0.f, 0.f, 150.f);

	FHitResult Hit;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.bReturnPhysicalMaterial = true;

	const bool bHit = GetWorld()->LineTraceSingleByChannel(
		Hit,
		Start,
		End,
		ECC_Visibility,
		Params
	);

	if (!bHit) return;

	const EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());

	TObjectPtr<USoundBase>* FoundSound = FootstepSounds.Find(SurfaceType);

	if (USoundBase* Sound = FoundSound ? *FoundSound : DefaultFootstepSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			Sound,
			Hit.ImpactPoint,
			FRotator::ZeroRotator,
			1.0f,
			1.0f,
			0.0f,
			FootstepSoundAttenuation
		);
	}

	UAISense_Hearing::ReportNoiseEvent(
		GetWorld(),
		Hit.ImpactPoint,
		RunNoiseLoudness,
		this,
		RunNoiseRange,
		TEXT("Footstep")
	);
}

void AThiefCharacter::DoMove(float Right, float Forward)
{
	if (!IsValid(ThiefComponent)) return;
	if (!ThiefComponent->bIsAlive) return;
	
	Super::DoMove(Right, Forward);
}

void AThiefCharacter::ServerKick_Implementation()
{
	if (!bDoAnimationDone) return;
	if (GetCharacterMovement()->IsFalling()) return;
	if (!IsValid(ThiefComponent)) return;
	if (!ThiefComponent->bIsAlive) return;
	
	const float Now = GetWorld()->GetTimeSeconds();
	if (Now - LastKickTime < KickCooldown) return;
	LastKickTime = Now;
	
	Multicast_Kick();
	
	TArray<AActor*> Overlapping;
	KickReach->GetOverlappingActors(Overlapping);
		
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
	if (!IsValid(ThiefComponent)) return;
	if (!ThiefComponent->bIsAlive) return;
	
	ThiefComponent->bIsSprint = bIsSprint;
}

bool AThiefCharacter::ServerSprint_Validate(bool bIsSprint)
{
	return true;
}

void AThiefCharacter::UpdatePlayerNameWidget()
{
	if (!IsValid(NameWidgetComponent)) return;
	NameWidgetComponent->InitWidget();

	AThiefPlayerState* PS = GetPlayerState<AThiefPlayerState>();

	UPlayerHeadNametag* NameWidget = Cast<UPlayerHeadNametag>(NameWidgetComponent->GetUserWidgetObject());
	
	if (!IsValid(PS)) return;
	if (!IsValid(NameWidget)) return;
	
	NameWidget->SetDisplayName(PS->GetDisplayName());
}

void AThiefCharacter::UpdateNameVisibility()
{
	if (!IsValid(NameWidgetComponent)) return;

	APawn* LocalPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!IsValid(LocalPawn)) return;

	if (LocalPawn == this)
	{
		NameWidgetComponent->SetVisibility(false);
		return;
	}

	const float DistanceSquared = FVector::DistSquared(
		LocalPawn->GetActorLocation(),
		GetActorLocation()
	);

	const bool bShouldShow = DistanceSquared <= FMath::Square(NameVisibleDistance);
	NameWidgetComponent->SetVisibility(bShouldShow);
}