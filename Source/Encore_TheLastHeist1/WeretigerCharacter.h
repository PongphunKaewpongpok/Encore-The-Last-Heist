// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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
	
	UPROPERTY()
	FString Form = TEXT("Human");
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="Skeletal Mesh")
	USkeletalMesh* HumanFormMesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="Skeletal Mesh")
	USkeletalMesh* TigerFormMesh;

	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float HumanFormSpeed = 125.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float TigerFormChaseSpeed = 700.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float TigerFormWanderSpeed = 200.0f;
	
	UFUNCTION(BlueprintPure)
	FString GetForm();
	UFUNCTION(BlueprintCallable)
	void SwitchForm();
};
