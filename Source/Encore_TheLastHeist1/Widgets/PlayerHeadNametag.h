// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHeadNametag.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class ENCORE_THELASTHEIST1_API UPlayerHeadNametag : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void SetDisplayName(const FText& NewName);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> DisplayNameText;
};
