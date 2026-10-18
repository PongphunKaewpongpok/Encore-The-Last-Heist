// Fill out your copyright notice in the Description page of Project Settings.


#include "WeretigerCharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"

AWeretigerCharacter::AWeretigerCharacter()
{
	TigerMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TigerMesh"));
	TigerMesh->SetupAttachment(GetCapsuleComponent());
	TigerMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -95.0f));
	TigerMesh->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
}

FString AWeretigerCharacter::GetForm()
{
	return Form;
}

void AWeretigerCharacter::SwitchForm()
{
	GetCharacterMovement()->MaxWalkSpeed = 0.0f;
	
	if (Form.Equals("Human"))
	{
		Form = TEXT("Tiger");
	}
	else
	{
		Form = TEXT("Human");
	}
}

void AWeretigerCharacter::Execute(AThiefCharacter* Target)
{
	Target->GotExecuted(this);
	
	
	AAIController* AICon = Cast<AAIController>(GetController());
	if (IsValid(AICon) && IsValid(AICon->BrainComponent))
	{
		AICon->BrainComponent->PauseLogic(TEXT("Execution Paused"));
	}
	
	
	
	FVector Start = GetActorLocation();
	FVector TargetLocation = Target->GetActorLocation();

	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(
		Start,
		TargetLocation
	);

	SetActorRotation(FRotator(
		0.f,
		LookAtRotation.Yaw,
		0.f
	));
	
	
	UAnimInstance* AnimInstance = TigerMesh->GetAnimInstance();
	if (!AnimInstance) return;
	
	if (ExecuteAnimation)
	{
		AnimInstance->Montage_Play(ExecuteAnimation);
		
		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &AWeretigerCharacter::OnExecuteEnded);

		AnimInstance->Montage_SetEndDelegate(EndDelegate, ExecuteAnimation);
	}
}

void AWeretigerCharacter::OnExecuteEnded(UAnimMontage* Montage, bool bInterrupted)
{
	AAIController* AICon = Cast<AAIController>(GetController());
	if (!AICon) return;

	AICon->BrainComponent->ResumeLogic(TEXT("Execution Finished"));
}
