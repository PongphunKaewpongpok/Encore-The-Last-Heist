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
	
	bool bRollingDone = true;
	void StartRolling();
	void OnRollingFinished(UAnimMontage* Montage, bool bInterrupted);
	UPROPERTY(EditAnywhere, Category="Animation")
	UAnimMontage* RollingAnimation;
	
public:
	virtual void DoMove(float Right, float Forward) override;
	virtual void Landed(const FHitResult& Hit) override;
	
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSprint(bool bIsSprint);
	
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerKick();
};
