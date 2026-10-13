// Fill out your copyright notice in the Description page of Project Settings.


#include "ThiefPlayerController.h"

#include "Blueprint/UserWidget.h"

void AThiefPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (!IsLocalController()) return;
	
	if (ItemOnGroundTabWidgetClass)
	{
		ItemOnGroundTabWidget = CreateWidget<UItemOnGroundTab>(
			this,
			ItemOnGroundTabWidgetClass
		);

		if (ItemOnGroundTabWidget)
		{
			ItemOnGroundTabWidget->AddToViewport();
		}
	}
}