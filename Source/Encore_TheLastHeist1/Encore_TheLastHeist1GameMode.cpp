// Copyright Epic Games, Inc. All Rights Reserved.

#include "Encore_TheLastHeist1GameMode.h"

#include "EngineUtils.h"
#include "Engine/TriggerBox.h"
#include "Components/BoxComponent.h"
#include "InteractableActor.h"
#include "Components/StaticMeshComponent.h"

void AEncore_TheLastHeist1GameMode::BeginPlay()
{
    Super::BeginPlay();
    
    for (TActorIterator<ATriggerBox> It(GetWorld()); It; ++It)
    {
        if (ATriggerBox* Box = *It; Box && Box->ActorHasTag(TEXT("ItemSpawnBox")))
        {
            SpawnableArea.Add(Box);
        }
    }
    
    SpawnRandomItems(30);
}

AEncore_TheLastHeist1GameMode::AEncore_TheLastHeist1GameMode()
{
	// stub
}

void AEncore_TheLastHeist1GameMode::SpawnRandomItems(const int SpawnCount)
{
    if (!GetWorld() || SpawnableArea.IsEmpty() || SpawnableItems.IsEmpty()) { return; }

    for (int32 i = 0; i < SpawnCount; ++i)
    {
        ATriggerBox* Box = SpawnableArea[FMath::RandRange(0, SpawnableArea.Num() - 1)];
        if (!Box) continue;

        UBoxComponent* BoxComponent = Cast<UBoxComponent>(Box->GetCollisionComponent());
        if (!BoxComponent) continue;

        const FVector Extent = BoxComponent->GetUnscaledBoxExtent();
        const FTransform Transform = BoxComponent->GetComponentTransform();

        const FVector LocalPosition(
            FMath::FRandRange(-Extent.X, Extent.X),
            FMath::FRandRange(-Extent.Y, Extent.Y),
            0.0f
        );

        const FVector RandomPosition = Transform.TransformPosition(LocalPosition);
        const FVector TraceStart = RandomPosition + FVector(0.0f, 0.0f, 10000.0f);
        const FVector TraceEnd = RandomPosition - FVector(0.0f, 0.0f, 10000.0f);

        FHitResult Hit;

        FCollisionQueryParams Params;
        Params.AddIgnoredActor(Box);

        const bool bHit = GetWorld()->LineTraceSingleByChannel(
            Hit,
            TraceStart,
            TraceEnd,
            ECC_Visibility,
            Params
        );
        if (!bHit) continue;

        UItemDataAsset* Item = SpawnableItems[FMath::RandRange(0, SpawnableItems.Num() - 1)];
        if (!Item || !Item->Mesh) continue;

        AInteractableActor* SpawnedActor =
            GetWorld()->SpawnActor<AInteractableActor>(
                InteractableClass,
                Hit.Location + FVector(0.0f, 0.0f, 5.0f),
                FRotator::ZeroRotator
            );
        
        if (IsValid(SpawnedActor) && IsValid(Item) && IsValid(Item->Mesh))
        {
            SpawnedActor->ItemData = Item;
            SpawnedActor->SetItemMesh(Item->Mesh);
        }
    }
}