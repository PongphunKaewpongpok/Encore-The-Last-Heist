// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FullBlackScreenFadable.generated.h"


class UBorder;
/**
 * 
 */
UCLASS()
class ENCORE_THELASTHEIST1_API UFullBlackScreenFadable : public UUserWidget
{
	GENERATED_BODY()
	
	
	FTimerHandle FadeTimerHandle;
	float FadeElapsed = 0.0f;
	float FadeDuration = 1.0f;
	float FadeStartOpacity = 1.0f;
	float FadeTargetOpacity = 0.0f;
	
protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<UBorder> BlackScreenBorder;
	
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFadeFinished);

	UPROPERTY(BlueprintAssignable)
	FOnFadeFinished OnFadeFinished;
	
	
	void FadeToBlackScreen(const float Duration = 1.0f);
	void FadeToNormalScreen(const float Duration = 1.0f);
	void UpdateFade();
};
