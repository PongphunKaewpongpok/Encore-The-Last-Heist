// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseAIController.h"
#include "WeretigerAIController.generated.h"

/**
 * 
 */
UCLASS()
class ENCORE_THELASTHEIST1_API AWeretigerAIController : public ABaseAIController
{
	GENERATED_BODY()
	
	FTimerHandle FormChangedTimerHandle;
	void FormChangedEvent(); 
	
protected:
	void OnTargetSensed(AActor* Actor, FAIStimulus Stimulus);
};
