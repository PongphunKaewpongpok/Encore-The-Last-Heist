// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Widgets/ItemOnGroundTab.h"
#include "Widgets/ItemDetailBar.h"

#include "CoreMinimal.h"
#include "Encore_TheLastHeist1PlayerController.h"
#include "ThiefPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ENCORE_THELASTHEIST1_API AThiefPlayerController : public AEncore_TheLastHeist1PlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UItemOnGroundTab> ItemOnGroundTabWidgetClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UItemDetailBar> ItemDetailBarWidgetClass;
	
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UItemOnGroundTab> ItemOnGroundTabWidget;
};
