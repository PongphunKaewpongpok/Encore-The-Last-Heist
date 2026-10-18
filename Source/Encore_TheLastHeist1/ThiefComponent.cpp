// Fill out your copyright notice in the Description page of Project Settings.


#include "ThiefComponent.h"

#include "Net/UnrealNetwork.h"
#include "ThiefPlayerController.h"
#include "ThiefPlayerState.h"

void UThiefComponent::OnRep_Items()
{
	RefreshInventoryUI();
}

void UThiefComponent::OnRep_Storage()
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
	TargetPC->MainUIWidget->SetStorageMoney(GetStorageMoney());
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
	DOREPLIFETIME(UThiefComponent, bIsAlive);
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

int32 UThiefComponent::GetStorageMoney()
{
	int32 TotalStorageMoney = 0;

	for (UItemDataAsset* Item : Storage)
	{
		if (!IsValid(Item)) continue;

		TotalStorageMoney += Item->Price;
	}

	return TotalStorageMoney;
}

void UThiefComponent::AddInventory(UItemDataAsset* Item)
{
	AThiefCharacter* Me = Cast<AThiefCharacter>(GetOwner());
	if (!IsValid(Me)) return;
	if (!Me->HasAuthority()) return;
	
	if (!IsValid(Item)) return;

	Items.Add(Item);
	UpdatePlayerState();
	
	RefreshInventoryUI();
}

UItemDataAsset* UThiefComponent::RemoveInventoryAt(const int32 Index)
{
	AThiefCharacter* Me = Cast<AThiefCharacter>(GetOwner());
	if (!IsValid(Me)) return nullptr;
	if (!Me->HasAuthority()) return nullptr;
	
	if (!Items.IsValidIndex(Index)) return nullptr;

	UItemDataAsset* RemovedItem = Items[Index];
	Items.RemoveAt(Index);
	UpdatePlayerState();
	
	RefreshInventoryUI();

	return RemovedItem;
}

void UThiefComponent::MoveToStorage(const int32 Index)
{
	UItemDataAsset* TargetItem = Items[Index];
	Storage.Add(TargetItem);
	UpdatePlayerState();
	
	RefreshInventoryUI();
}

void UThiefComponent::UpdatePlayerState()
{
	AThiefCharacter* Me = Cast<AThiefCharacter>(GetOwner());
	if (!IsValid(Me)) return;
	if (!Me->HasAuthority()) return;
	
	AThiefPlayerState* PS = Me->GetPlayerState<AThiefPlayerState>();
	if (!IsValid(PS)) return;
	
	PS->SetMoney(GetStorageMoney()+GetMoney());
}

