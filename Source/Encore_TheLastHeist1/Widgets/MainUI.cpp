// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/MainUI.h"

#include "Components/TextBlock.h"

void UMainUI::SetWeight(const float Weight)
{
	if (!WeightValueText) return;
	
	WeightValueText->SetText(FText::FromString(FString::SanitizeFloat(Weight) + TEXT(" kg")));
	
	float Alpha = FMath::Clamp(Weight / 70.f, 0.f, 1.f);

	FLinearColor Color = Alpha < 0.5f
		? FMath::Lerp(FLinearColor::Green, FLinearColor::Yellow, Alpha * 2.f)
		: FMath::Lerp(FLinearColor::Yellow, FLinearColor::Red, (Alpha - 0.5f) * 2.f);

	WeightValueText->SetColorAndOpacity(Color);
}

void UMainUI::SetMoney(const int32 Money)
{
	if (!MoneyValueText) return;
	
	MoneyValueText->SetText(FText::AsNumber(Money));
}

void UMainUI::SetBankMoney(const int32 BankMoney)
{
	if (!BankMoneyValueText) return;
	
	BankMoneyValueText->SetText(FText::AsNumber(BankMoney));
}
