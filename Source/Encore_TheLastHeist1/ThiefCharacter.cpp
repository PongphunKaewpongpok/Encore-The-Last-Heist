// Fill out your copyright notice in the Description page of Project Settings.


#include "ThiefCharacter.h"
#include "Engine/LocalPlayer.h"
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
#include "Widgets/PlayerHeadNametag.h"






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
	const bool bSprint = Value.Get<bool>();

	ThiefComponent->bIsSprint = bSprint;

	ServerSprint(bSprint);
}

void AThiefCharacter::DoKick(const FInputActionValue& Value)
{
	ServerKick();
}

void AThiefCharacter::OnMouseWheel(const FInputActionValue& Value)
{
	if (!IsLocallyControlled()) return;
	
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
	AThiefPlayerController* TargetPC = Cast<AThiefPlayerController>(GetController());
	AInteractableActor* TargetItem = TargetPC->ItemOnGroundTabWidget->GetSelectedInteractableActor();
	
	TargetPC->ItemOnGroundTabWidget->CollectItem();
	ServerInteract(TargetItem);
}




void AThiefCharacter::UpdateSprint()
{
	if (!bDoAnimationDone) return;
	
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

void AThiefCharacter::DropItem(const int32 Index)
{
	ThiefComponent->RemoveInventoryAt(Index);
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
	if (!bDoAnimationDone) return;
	if (GetCharacterMovement()->IsFalling()) return;
	
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
	ThiefComponent->bIsSprint = bIsSprint;
}

bool AThiefCharacter::ServerSprint_Validate(bool bIsSprint)
{
	return true;
}

void AThiefCharacter::UpdatePlayerNameWidget()
{
	if (!IsValid(NameWidgetComponent))
		return;

	NameWidgetComponent->InitWidget();

	AThiefPlayerState* PS = GetPlayerState<AThiefPlayerState>();

	UPlayerHeadNametag* NameWidget =
		Cast<UPlayerHeadNametag>(
			NameWidgetComponent->GetUserWidgetObject()
		);

	if (!IsValid(PS))
	{
		UE_LOG(LogTemp, Warning,
			TEXT("PlayerState is NULL: %s"),
			*GetNameSafe(this));
		return;
	}

	if (!IsValid(NameWidget))
	{
		UE_LOG(LogTemp, Warning,
			TEXT("NameWidget is NULL: %s"),
			*GetNameSafe(this));
		return;
	}

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