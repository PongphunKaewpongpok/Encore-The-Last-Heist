// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
protected:
	const float SprintNoiseRange = 100.0f;
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* SprintAction;
	
	void ToggleSprint(const FInputActionValue& Value);
	void DoKick(const FInputActionValue& Value);
	
public:
	virtual void DoMove(float Right, float Forward) override;
	
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSprint(bool bIsSprint);
	
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerKick();
};
