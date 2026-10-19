// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ThiefCharacter.h"

#include "CoreMinimal.h"
#include "Encore_TheLastHeist1Character.h"
#include "WeretigerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class ENCORE_THELASTHEIST1_API AWeretigerCharacter : public AEncore_TheLastHeist1Character
{
	GENERATED_BODY()
	
	UPROPERTY(Replicated)
	FString Form = TEXT("Human");
	
protected:
	AWeretigerCharacter();

	
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category ="Skeletal Mesh")
	USkeletalMeshComponent* TigerMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float HumanFormSpeed = 125.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float TigerFormChaseSpeed = 700.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float TigerFormWanderSpeed = 200.0f;
	
	UPROPERTY(EditDefaultsOnly, Category="AI")
	FName TargetActorKey = TEXT("TargetActor");
	
	UPROPERTY(EditAnywhere, Category="Animation")
	UAnimMontage* StartTranformToTigerAnimation;
	UPROPERTY(EditAnywhere, Category="Animation")
	UAnimMontage* ExecuteAnimation;
	
	
	
	UFUNCTION(BlueprintPure)
	FString GetForm();
	UFUNCTION(BlueprintCallable)
	void SwitchForm();
	
	void Execute(AThiefCharacter* Target);
	void OnExecuteEnded(UAnimMontage* Montage, bool bInterrupted);
	void OnStartTranformToTigerEnded(UAnimMontage* Montage, bool bInterrupted);
	
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_StartTranformToTiger();
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_Execute();
};
