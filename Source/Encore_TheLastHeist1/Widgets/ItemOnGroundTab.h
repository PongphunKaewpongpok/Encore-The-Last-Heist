// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ItemDataAsset.h"
#include "Widgets/ItemDetailBar.h"
#include "InteractableActor.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemOnGroundTab.generated.h"


class UWrapBox;
class UBorder;
/**
 * 
 */
UCLASS()
class ENCORE_THELASTHEIST1_API UItemOnGroundTab : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
public:
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<UWrapBox> WrapBox;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<UBorder> ItemListMenu;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UItemDetailBar> ItemDetailBarClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
	TObjectPtr<USoundBase> Sound_CollectItem;
	
	UPROPERTY(BlueprintReadOnly)
	int32 SelectedSlot = 0;
	
	
	void ItemBarScrollingDown();
	void ItemBarScrollingUp();
	void CollectItem();
	void AddItemBar(AInteractableActor* InteractableActor);
    void RemoveItemBar(AInteractableActor* InteractableActor);
	void RemoveItemBarAt(int32 Index);
	void UpdateSelectingItem();
	
	UFUNCTION(BlueprintPure)
	UItemDataAsset* GetSelectedItemData();
	UFUNCTION(BlueprintPure)
	AInteractableActor* GetSelectedInteractableActor();
	UFUNCTION(BlueprintPure)
	UItemDetailBar* GetSelectedItemDetailBarWidget();
	UFUNCTION(BlueprintPure)
	bool IsEmpty() const;
};
