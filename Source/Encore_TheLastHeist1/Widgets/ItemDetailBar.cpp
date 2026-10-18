// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/ItemDetailBar.h"


#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Components/Image.h"


void UItemDetailBar::NativeConstruct()
{
	Super::NativeConstruct();
}

void UItemDetailBar::Setup(AInteractableActor* InInteractableActor)
{
	InteractableActor = InInteractableActor;

	
	FText WeightResult = FText::Format(
		FText::FromString(TEXT("{0} kg")),
		FText::AsNumber(InteractableActor->ItemData->Weight)
	);
	
	ItemNameText->SetText(InteractableActor->ItemData->ItemName);
	WeightValueText->SetText(WeightResult);
	PriceValueText->SetText(FText::AsNumber(InteractableActor->ItemData->Price));
}

void UItemDetailBar::SetSelected(bool bSelect)
{
	const FLinearColor Color = bSelect ?
		FLinearColor(1.0f, 1.0f, 0.0f, 1.0f) : 
		FLinearColor(0.0f, 0.0f, 0.0f, 0.0f);
	
	InteractIcon->SetVisibility(bSelect ?
		ESlateVisibility::Visible :
		ESlateVisibility::Hidden);
	
	Border1->SetBrushColor(Color);
	Border2->SetBrushColor(Color);
	Border3->SetBrushColor(Color);
	Border4->SetBrushColor(Color);
	}
