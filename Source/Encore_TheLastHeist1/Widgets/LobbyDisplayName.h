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
	virtual void NativeConstruct() override;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<UTextBlock> DisplayNameText;
public:
	UFUNCTION()
	void HandleDataChanged();

	void BindPlayerState();
	
	void SetDisplayName(const FText& NewDisplayName) const;
};
