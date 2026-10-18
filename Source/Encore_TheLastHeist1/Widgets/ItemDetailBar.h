// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InteractableActor.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemDetailBar.generated.h"


class UTextBlock;
class UImage;
class UBorder;
/**
 * 
 */
UCLASS()
class ENCORE_THELASTHEIST1_API UItemDetailBar : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	
	
public:
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<UTextBlock> WeightValueText;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<UTextBlock> PriceValueText;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<UTextBlock> ItemNameText;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<UImage> InteractIcon;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<UBorder> Border1;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<UBorder> Border2;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<UBorder> Border3;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<UBorder> Border4;
	
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<AInteractableActor> InteractableActor;
	
	void Setup(AInteractableActor* InteractableActor);
	void SetSelected(bool bSelect);
};
