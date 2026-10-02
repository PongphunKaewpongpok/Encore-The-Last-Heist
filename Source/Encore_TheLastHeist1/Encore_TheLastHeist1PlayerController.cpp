// Copyright Epic Games, Inc. All Rights Reserved.


#include "Encore_TheLastHeist1PlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "Blueprint/UserWidget.h"
#include "Encore_TheLastHeist1.h"
#include "Widgets/Input/SVirtualJoystick.h"
/*#inclide "AEncore_TheLastHeist1PlayerState.h"*/



FGenericTeamId AEncore_TheLastHeist1PlayerController::GetGenericTeamId() const
{
	/*if (const AEncore_TheLastHeist1PlayerState* PS = GetPlayerState<AEncore_TheLastHeist1PlayerState>())
	{
		return FGenericTeamId(PS->TeamId);
	}*/

	return FGenericTeamId::NoTeam;
}

void AEncore_TheLastHeist1PlayerController::BeginPlay()
{
	Super::BeginPlay();

	// only spawn touch controls on local player controllers
	if (ShouldUseTouchControls() && IsLocalPlayerController())
	{
		// spawn the mobile controls widget
		MobileControlsWidget = CreateWidget<UUserWidget>(this, MobileControlsWidgetClass);

		if (MobileControlsWidget)
		{
			// add the controls to the player screen
			MobileControlsWidget->AddToPlayerScreen(0);

		} else {

			UE_LOG(LogEncore_TheLastHeist1, Error, TEXT("Could not spawn mobile controls widget."));

		}

	}
}

void AEncore_TheLastHeist1PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// only add IMCs for local player controllers
	if (IsLocalPlayerController())
	{
		// Add Input Mapping Contexts
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
			{
				Subsystem->AddMappingContext(CurrentContext, 0);
			}

			// only add these IMCs if we're not using mobile touch input
			if (!ShouldUseTouchControls())
			{
				for (UInputMappingContext* CurrentContext : MobileExcludedMappingContexts)
				{
					Subsystem->AddMappingContext(CurrentContext, 0);
				}
			}
		}
	}
}

bool AEncore_TheLastHeist1PlayerController::ShouldUseTouchControls() const
{
	// are we on a mobile platform? Should we force touch?
	return SVirtualJoystick::ShouldDisplayTouchInterface() || bForceTouchControls;
}
