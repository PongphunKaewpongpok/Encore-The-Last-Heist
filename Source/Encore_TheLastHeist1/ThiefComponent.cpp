// Fill out your copyright notice in the Description page of Project Settings.


#include "ThiefComponent.h"

#include "Net/UnrealNetwork.h"
#include "ThiefPlayerController.h"

void UThiefComponent::OnRep_Items()
{
	RefreshInventoryUI();
}

void UThiefComponent::RefreshInventoryUI()
{
	AThiefCharacter* Me = Cast<AThiefCharacter>(GetOwner());
	if (!Me || !Me->IsLocallyControlled()) return;

	AThiefPlayerController* TargetPC = Cast<AThiefPlayerController>(Me->GetController());
	if (!TargetPC || !TargetPC->MainUIWidget) return;

	TargetPC->MainUIWidget->SetWeight(GetWeight());
	TargetPC->MainUIWidget->SetMoney(GetMoney());
}

// Sets default values for this component's properties
UThiefComponent::UThiefComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
}


// Called when the game starts
void UThiefComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UThiefComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UThiefComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UThiefComponent, Items);
	DOREPLIFETIME(UThiefComponent, bIsSprint);
}

TArray<TObjectPtr<UItemDataAsset>>& UThiefComponent::GetInventory()
{
	return Items;
}

float UThiefComponent::GetWeight()
{
	float TotalWeight = 0.0f;

	for (UItemDataAsset* Item : GetInventory())
	{
		if (!IsValid(Item)) continue;

		TotalWeight += Item->Weight;
	}

	return TotalWeight;
}

int32 UThiefComponent::GetMoney()
{
	int32 TotalMoney = 0;

	for (UItemDataAsset* Item : GetInventory())
	{
		if (!IsValid(Item)) continue;

		TotalMoney += Item->Price;
	}

	return TotalMoney;
}

void UThiefComponent::AddInventory(UItemDataAsset* Item)
{
	if (!IsValid(Item)) return;

	Items.Add(Item);
	
	RefreshInventoryUI();
}

void UThiefComponent::RemoveInventoryAt(const int32 Index)
{
	if (Items.IsValidIndex(Index))
	{
		Items.RemoveAt(Index);
	}
}

