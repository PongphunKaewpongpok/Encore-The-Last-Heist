// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyMainUI.generated.h"

class ALobbyPlayerState;
class UScrollBox;
class ULobbyDisplayNameBar;
/**
 * 
 */
UCLASS()
class ENCORE_THELASTHEIST1_API ULobbyMainUI : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<UScrollBox> DisplayNameListTab;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<ULobbyDisplayNameBar> LobbyDisplayNameBarClass;
	
public:
	UFUNCTION()
	void HandleDataChanged();

	void BindSinglePlayerState(ALobbyPlayerState* LobbyPS);
	
	void RefreshDisplayNameTab();
};
