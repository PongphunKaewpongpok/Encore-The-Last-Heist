// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyDisplayName.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class ENCORE_THELASTHEIST1_API ULobbyDisplayName : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<UTextBlock> DisplayNameText;
public:
	void SetDisplayName(const FText& NewDisplayName) const;
};
