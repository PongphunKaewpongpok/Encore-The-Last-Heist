// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ItemDataAsset.h"
#include "Widgets/ItemDetailBar.h"
#include "InteractableActor.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemOnGroundTab.generated.h"

/**
 * 
 */
UCLASS()
class ENCORE_THELASTHEIST1_API UItemOnGroundTab : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ItemBarScrollingDown();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ItemBarScrollingUp();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void CollectItem();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AddItemBar(AInteractableActor* InteractableActor);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    void RemoveItemBar(AInteractableActor* InteractableActor);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void RemoveItemBarAt(int32 Index);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
	UItemDataAsset* GetSelectedItemData();
};
