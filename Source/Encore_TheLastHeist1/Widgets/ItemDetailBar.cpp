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

	const TMap<FString, FLinearColor> RarityColors = {
		{ TEXT("S"), FLinearColor(1.0f, 0.75f, 0.0f) },
		{ TEXT("A"), FLinearColor(0.6f, 0.2f, 1.0f) },
		{ TEXT("B"), FLinearColor(0.0f, 0.6f, 1.0f) },
		{ TEXT("C"), FLinearColor(1.0f, 1.0f, 1.0f) }
	};
	
	FText WeightResult = FText::Format(
		FText::FromString(TEXT("{0} kg")),
		FText::AsNumber(InteractableActor->ItemData->Weight)
	);
	
	ItemNameText->SetText(InteractableActor->ItemData->ItemName);
	WeightValueText->SetText(WeightResult);
	PriceValueText->SetText(FText::AsNumber(InteractableActor->ItemData->Price));
	
	ItemImage->SetBrushFromTexture(InteractableActor->ItemData->Image);
	
	const FString Rarity = InteractableActor->ItemData->Rarity.ToString();
	if (const FLinearColor* Color = RarityColors.Find(Rarity))
	{
		RarityColorBorder->SetBrushColor(*Color);
	}
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
