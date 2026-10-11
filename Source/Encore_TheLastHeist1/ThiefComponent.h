// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ItemDataAsset.h"

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ThiefComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ENCORE_THELASTHEIST1_API UThiefComponent : public UActorComponent
{
	GENERATED_BODY()
	
	TArray<TObjectPtr<UItemDataAsset>> Items;

public:	
	// Sets default values for this component's properties
	UThiefComponent();

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool bIsSprint = false;
	
	

	UFUNCTION(BlueprintCallable, Category="Inventory")
	TArray<UItemDataAsset*> GetInventory() const;

	UFUNCTION(BlueprintCallable, Category="Inventory")
	void AddInventory(UItemDataAsset* NewItem);
};
