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
	
	UPROPERTY(ReplicatedUsing = OnRep_Items)
	TArray<TObjectPtr<UItemDataAsset>> Items;
	UFUNCTION()
	void OnRep_Items();
	
	UPROPERTY(ReplicatedUsing = OnRep_Storage)
	TArray<TObjectPtr<UItemDataAsset>> Storage;
	UFUNCTION()
	void OnRep_Storage();
	
	void RefreshInventoryUI();

public:	
	// Sets default values for this component's properties
	UThiefComponent();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(Replicated)
	bool bIsSprint = false;
	
	
	TArray<TObjectPtr<UItemDataAsset>>& GetInventory();
	UFUNCTION(BlueprintCallable, Category="Inventory")
	float GetWeight();
	UFUNCTION(BlueprintCallable, Category="Inventory")
	int32 GetMoney();
	UFUNCTION(BlueprintCallable, Category="Inventory")
	int32 GetStorageMoney();

	UFUNCTION(BlueprintCallable, Category="Inventory")
	void AddInventory(UItemDataAsset* Item);
	UFUNCTION(BlueprintCallable, Category="Inventory")
	UItemDataAsset* RemoveInventoryAt(const int32 Index);
	
	UFUNCTION(BlueprintCallable, Category="Inventory")
	void MoveToStorage(const int32 Index);
	
	void UpdatePlayerState();
};
