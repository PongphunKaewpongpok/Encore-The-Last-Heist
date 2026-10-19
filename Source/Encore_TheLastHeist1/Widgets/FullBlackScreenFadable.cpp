// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/FullBlackScreenFadable.h"

#include "Components/Border.h"

void UFullBlackScreenFadable::FadeToBlackScreen(const float Duration)
{
	if (!IsValid(BlackScreenBorder)) return;

	GetWorld()->GetTimerManager().ClearTimer(FadeTimerHandle);

	FadeElapsed = 0.0f;
	FadeDuration = FMath::Max(Duration, 0.001f);
	FadeStartOpacity = BlackScreenBorder->GetRenderOpacity();
	FadeTargetOpacity = 1.0f;

	GetWorld()->GetTimerManager().SetTimer(
		FadeTimerHandle,
		this,
		&UFullBlackScreenFadable::UpdateFade,
		0.016f,
		true
	);
}

void UFullBlackScreenFadable::FadeToNormalScreen(const float Duration)
{
	if (!IsValid(BlackScreenBorder)) return;

	GetWorld()->GetTimerManager().ClearTimer(FadeTimerHandle);

	FadeElapsed = 0.0f;
	FadeDuration = FMath::Max(Duration, 0.001f);
	FadeStartOpacity = BlackScreenBorder->GetRenderOpacity();
	FadeTargetOpacity = 0.0f;

	GetWorld()->GetTimerManager().SetTimer(
		FadeTimerHandle,
		this,
		&UFullBlackScreenFadable::UpdateFade,
		0.016f,
		true
	);
}

void UFullBlackScreenFadable::UpdateFade()
{
	if (!IsValid(BlackScreenBorder))
	{
		GetWorld()->GetTimerManager().ClearTimer(FadeTimerHandle);
		return;
	}

	FadeElapsed += 0.016f;

	const float Alpha = FMath::Clamp(
		FadeElapsed / FadeDuration,
		0.0f,
		1.0f
	);

	const float Opacity = FMath::Lerp(
		FadeStartOpacity,
		FadeTargetOpacity,
		Alpha
	);

	BlackScreenBorder->SetRenderOpacity(Opacity);

	if (Alpha >= 1.0f)
	{
		BlackScreenBorder->SetRenderOpacity(FadeTargetOpacity);
		GetWorld()->GetTimerManager().ClearTimer(FadeTimerHandle);
		
		UE_LOG(LogTemp, Warning, TEXT("Start BC..."))
		OnFadeFinished.Broadcast();
	}
}
