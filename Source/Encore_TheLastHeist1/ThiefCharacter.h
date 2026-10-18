// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ThiefComponent.h"

#include "CoreMinimal.h"
#include "Encore_TheLastHeist1Character.h"
#include "ThiefCharacter.generated.h"


class AInteractableActor;
class UWidgetComponent;
class UPlayerNameWidget;
class AWeretigerCharacter;

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
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;
	virtual void OnRep_PlayerState() override;
    virtual void PossessedBy(AController* NewController) override;
	
protected:
	
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UThiefComponent> ThiefComponent;
	
	const float SprintNoiseRange = 100.0f;
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* SprintAction;
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* KickAction;
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MouseWheelAction;
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* InteractAction;
	
	void ToggleSprint(const FInputActionValue& Value);
	void DoKick(const FInputActionValue& Value);
	void OnMouseWheel(const FInputActionValue& Value);
	void DoInteract(const FInputActionValue& Value);
	
	void UpdateSprint();
	const float WalkingSpeed = 100.0f;
	const float RunningSpeed = 500.0f;
	
	bool bDoAnimationDone = true;
	void OnMontageFinished(UAnimMontage* Montage, bool bInterrupted);
	
	void StartRolling();
	UPROPERTY(EditAnywhere, Category="Animation")
	UAnimMontage* RollingAnimation;
	
	
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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kick")
	TArray<TObjectPtr<UStaticMesh>> KickableMeshes;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Name")
	TObjectPtr<UWidgetComponent> NameWidgetComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Name")
	TSubclassOf<UUserWidget> PlayerNameWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Name")
	float NameVisibleDistance = 750.0f;

	void UpdatePlayerNameWidget();
	void UpdateNameVisibility();
	
	UPROPERTY(EditAnywhere, Category="Animation")
	UAnimMontage* DeathAnimation;
	
public:
	virtual void DoMove(float Right, float Forward) override;
	virtual void Landed(const FHitResult& Hit) override;
	
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSprint(bool bIsSprint);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerKick();
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerInteract(AInteractableActor* TargetItem);
	
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_Knockdown(const FVector& Direction, const float Strength);
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_Kick();
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_RemoveOnGroundItem(AInteractableActor* InteractableActor);
	
	void DropItem(const int32 Index);
	void GotExecuted(AWeretigerCharacter* Weretiger);
	bool IsDead();
};
