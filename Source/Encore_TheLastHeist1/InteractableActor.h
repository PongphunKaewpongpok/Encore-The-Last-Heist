// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ItemDataAsset.h"
#include "ThiefCharacter.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableActor.generated.h"


class UStaticMeshComponent;
class USphereComponent;
class UMaterialInterface;

UCLASS()
class ENCORE_THELASTHEIST1_API AInteractableActor : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AInteractableActor();
	
	UFUNCTION()
	void OnBeginOverlapHighlight(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnEndOverlapHighlight(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UFUNCTION()
	void OnBeginOverlapCollectable(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnEndOverlapCollectable(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> StaticMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USphereComponent> HighlightCollision;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USphereComponent> CollectableCollision;
	
	
	UPROPERTY(Replicated, BlueprintReadWrite)
	TObjectPtr<UItemDataAsset> ItemData;
	
	UFUNCTION()
	void OnRep_ItemMesh();
	void SetItemMesh(UStaticMesh* NewMesh, const float Scale);
	UPROPERTY(ReplicatedUsing = OnRep_ItemMesh)
	TObjectPtr<UStaticMesh> ReplicatedItemMesh;
	UPROPERTY(Replicated)
	float ReplicatedScale;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UMaterialInterface> HighlightMaterial;
	
	void Interact(AThiefCharacter* Target);
	bool IsCanCollect(AThiefCharacter* Target) const;
};
