// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ThiefComponent.h"

#include "CoreMinimal.h"
#include "Encore_TheLastHeist1Character.h"
#include "ThiefCharacter.generated.h"

/**
 * 
 */
UCLASS()
class ENCORE_THELASTHEIST1_API AThiefCharacter : public AEncore_TheLastHeist1Character
{
	GENERATED_BODY()

	FTimerHandle KickTimerHandle;
	FTimerHandle KnockdownTimerHandle;
public:
	AThiefCharacter();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UThiefComponent> ThiefComponent;
	
	const float SprintNoiseRange = 100.0f;
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* SprintAction;
	
	void ToggleSprint(const FInputActionValue& Value);
	void DoKick(const FInputActionValue& Value);
	void UpdateSprint();
	
	bool bDoAnimationDone = true;
	void OnMontageFinished(UAnimMontage* Montage, bool bInterrupted);
	
	void StartRolling();
	UPROPERTY(EditAnywhere, Category="Animation")
	UAnimMontage* RollingAnimation;
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* KickAction;
	float LastKickTime = -1000.0f;
	void Knockdown(const FVector& Direction, const float Strength);
	UPROPERTY(EditAnywhere, Category="Animation")
	UAnimMontage* KickingAnimation;
	UPROPERTY(EditAnywhere, Category="Animation")
	UAnimMontage* GotKnockdownAnimation;
	UPROPERTY(EditAnywhere, Category="Kick")
	float KickRange = 150.f;
	UPROPERTY(EditAnywhere, Category="Kick") 
	float KickHalfAngle = 60.f;
	UPROPERTY(EditDefaultsOnly, Category="Kick")
	float KickCooldown = 10.0f;
	UPROPERTY(EditDefaultsOnly, Category="Kick")
	float KickStrength = 800.0f;
	UPROPERTY(VisibleAnywhere, Category="Kick")
	class USphereComponent* KickReach;
	
public:
	virtual void DoMove(float Right, float Forward) override;
	virtual void Landed(const FHitResult& Hit) override;
	
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSprint(bool bIsSprint);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerKick();
	
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_Knockdown(const FVector& Direction, const float Strength);
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_Kick();
};
