// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "LobbyThiefDummy.generated.h"

class ULobbyDisplayName;
class UWidgetComponent;

UCLASS()
class ENCORE_THELASTHEIST1_API ALobbyThiefDummy : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ALobbyThiefDummy();

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TObjectPtr<USkeletalMeshComponent> SkeletalMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
    TObjectPtr<UWidgetComponent> FloatingWidget;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<ULobbyDisplayName> LobbyDisplayNameWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	TObjectPtr<USkeletalMesh> ThiefSkeletalMesh;
	
public:	
	// Called every frame
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input

	void AddWidget(ULobbyDisplayName* TargetWidget);
};
