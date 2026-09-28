// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Weretiger_HumanChase.h"

#include "AIController.h"
#include "ThiefCharacter.h"
#include "WeretigerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTTask_Weretiger_HumanChase::UBTTask_Weretiger_HumanChase()
{
	NodeName = TEXT("BTTask Weretiger HumanChase");
}

EBTNodeResult::Type UBTTask_Weretiger_HumanChase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AICon = OwnerComp.GetAIOwner();
	if (AICon == nullptr) { return EBTNodeResult::Failed; }
	
	AWeretigerCharacter* Me = Cast<AWeretigerCharacter>(AICon->GetPawn());
	if (Me == nullptr) { return EBTNodeResult::Failed; }
	
	UBlackboardComponent* BB = AICon->GetBlackboardComponent();
	if (BB == nullptr) { return EBTNodeResult::Failed; }
	
	AThiefCharacter* Target = Cast<AThiefCharacter>(BB->GetValueAsObject(TargetActorKey.SelectedKeyName));
	if (Target == nullptr) { return EBTNodeResult::Failed; }
	
	
	if (Me->GetForm().Equals("Human"))
	{
		if (IsOnScreen(Target, Me))
		{
			Me->SwitchForm();
		}
		else
		{
			AICon->MoveToActor(Target);
		}
	}
	else
	{
		AICon->MoveToActor(Target);
	}
	
	return EBTNodeResult::Succeeded;
}

bool UBTTask_Weretiger_HumanChase::IsOnScreen(ACharacter* Me, ACharacter* Target)
{
	APlayerController* PC = Cast<APlayerController>(Me->GetController());
	if (PC == nullptr) { return false; }
	
	FVector WorldLocation = Me->GetActorLocation();
	FVector2D ScreenLocation;
	if (!PC->ProjectWorldLocationToScreen(WorldLocation, ScreenLocation))
	{
		return false;
	}
	
	int32 SizeX;
	int32 SizeY;
	PC->GetViewportSize(SizeX, SizeY);

	const bool bOnScreen =
		ScreenLocation.X >= 0.f &&
		ScreenLocation.X <= SizeX &&
		ScreenLocation.Y >= 0.f &&
		ScreenLocation.Y <= SizeY;
	
	if (!bOnScreen) { return false; }
	
	
	
	FVector CameraLocation;
	FRotator CameraRotation;

	PC->GetPlayerViewPoint(CameraLocation, CameraRotation);

	FHitResult Hit;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Me);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		Hit,
		CameraLocation,
		Target->GetActorLocation(),
		ECC_Visibility,
		Params
	);

	if (bHit && Hit.GetActor() != Target)
	{
		return false;
	}
	return true;
}
