// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemOnGroundTab.h"

#include "Components/Border.h"
#include "Components/WrapBox.h"
#include "Kismet/GameplayStatics.h"

void UItemOnGroundTab::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	if (!IsValid(WrapBox)) return;
	
	UpdateSelectingItem();
	ItemListMenu->SetVisibility(WrapBox->HasAnyChildren() ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UItemOnGroundTab::ItemBarScrollingDown()
{
	if (!IsValid(WrapBox)) return;
	
	if (SelectedSlot+1 > WrapBox->GetChildrenCount()-1) return;
	SelectedSlot++;
}

void UItemOnGroundTab::ItemBarScrollingUp()
{
	if (!IsValid(WrapBox)) return;
	
	if (SelectedSlot-1 < 0) return;
	SelectedSlot--;
}

UItemDataAsset* UItemOnGroundTab::GetSelectedItemData()
{
	AInteractableActor* TemInteractableActor = GetSelectedInteractableActor();
	if (!TemInteractableActor) return nullptr;
	
	return TemInteractableActor->ItemData;
}

void UItemOnGroundTab::CollectItem()
{
	UItemDataAsset* ItemData = GetSelectedItemData();
	if (!IsValid(ItemData)) return;
	
	AThiefCharacter* Me = Cast<AThiefCharacter>(GetOwningPlayerPawn());
	if (!IsValid(Me)) return;
	
	RemoveItemBarAt(SelectedSlot);
	if (Sound_CollectItem)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), Sound_CollectItem);
	}
	
	
	if (WrapBox->GetChildrenCount() == 0)
	{
		SelectedSlot = 0;
	}
	else
	{
		if (SelectedSlot >= WrapBox->GetChildrenCount())
		{
			SelectedSlot--;
		}
	}
	
}

void UItemOnGroundTab::AddItemBar(AInteractableActor* InteractableActor)
{
	if (!IsValid(WrapBox)) return;
	
	UItemDetailBar* ItemDetailBarWidget = CreateWidget<UItemDetailBar>(
		this,
		ItemDetailBarClass
	);
	
	if (IsValid(ItemDetailBarWidget))
	{
		ItemDetailBarWidget->Setup(InteractableActor);
	}
	
	WrapBox->AddChildToWrapBox(ItemDetailBarWidget);

}

void UItemOnGroundTab::RemoveItemBar(AInteractableActor* InteractableActor)
{
	if (!IsValid(WrapBox)) return;
	
	for (UWidget* Widget : WrapBox->GetAllChildren())
	{
		UItemDetailBar* ItemDetailBarWidget = Cast<UItemDetailBar>(Widget);
		
		if (ItemDetailBarWidget->InteractableActor == InteractableActor)
		{
			ItemDetailBarWidget->RemoveFromParent();
		}
	}
}

void UItemOnGroundTab::RemoveItemBarAt(int Index)
{
	if (!IsValid(WrapBox)) return;
	
	WrapBox->RemoveChildAt(Index);
}


bool UItemOnGroundTab::IsEmpty() const
{
	if (!IsValid(WrapBox)) return false;
	
	return !WrapBox->HasAnyChildren();
}

void UItemOnGroundTab::UpdateSelectingItem()
{
	if (!IsValid(WrapBox)) return;
	
	int32 Index = 0;
	for (UWidget* Widget : WrapBox->GetAllChildren())
	{
		UItemDetailBar* ItemDetailBarWidget = Cast<UItemDetailBar>(Widget);
		
		ItemDetailBarWidget->SetSelected(Index == SelectedSlot);
		Index++;
	}
	
}

AInteractableActor* UItemOnGroundTab::GetSelectedInteractableActor()
{
	UItemDetailBar* TemItemDetailBar = GetSelectedItemDetailBarWidget();
	if (!TemItemDetailBar) return nullptr;
	
	return TemItemDetailBar->InteractableActor;
}

UItemDetailBar* UItemOnGroundTab::GetSelectedItemDetailBarWidget()
{
	if (!IsValid(WrapBox)) return nullptr;
	
	if (SelectedSlot < 0 || SelectedSlot >= WrapBox->GetChildrenCount()) return nullptr;
	
	UItemDetailBar* ItemDetailBarWidget = Cast<UItemDetailBar>(WrapBox->GetChildAt(SelectedSlot));
	if (!IsValid(ItemDetailBarWidget)) return nullptr;

	return ItemDetailBarWidget;
}
