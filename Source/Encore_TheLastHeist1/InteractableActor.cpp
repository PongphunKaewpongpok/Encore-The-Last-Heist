// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableActor.h"

#include "Components/SphereComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Net/UnrealNetwork.h"
#include "Components/StaticMeshComponent.h"

#include "ThiefCharacter.h"
#include "ThiefPlayerController.h"
#include "Widgets/ItemOnGroundTab.h"


// Item Highlight
void AInteractableActor::OnBeginOverlapHighlight(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == this) return;
	
	AThiefCharacter* Target = Cast<AThiefCharacter>(OtherActor);
	if (!IsValid(Target)) return;
	
	
	
	if (Target->IsLocallyControlled())
	{
		if (!IsValid(StaticMesh) || !IsValid(HighlightMaterial)) return;
		StaticMesh->SetOverlayMaterial(HighlightMaterial);
	}
}

void AInteractableActor::OnEndOverlapHighlight(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!OtherActor || OtherActor == this) return;
	
	AThiefCharacter* Target = Cast<AThiefCharacter>(OtherActor);
	if (!IsValid(Target)) return;
	
	
	
	if (Target->IsLocallyControlled())
	{
		if (!IsValid(StaticMesh) || !IsValid(HighlightMaterial)) return;
		StaticMesh->SetOverlayMaterial(nullptr);
	}
}


// Item Collectable
void AInteractableActor::OnBeginOverlapCollectable(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == this) return;
	
	AThiefCharacter* Target = Cast<AThiefCharacter>(OtherActor);
	if (!IsValid(Target)) return;
	
	
	
	if (Target->IsLocallyControlled())
	{
		AThiefPlayerController* TargetPC = Cast<AThiefPlayerController>(Target->GetController());
		TargetPC->ItemOnGroundTabWidget->AddItemBar(this);
	}
}

void AInteractableActor::OnEndOverlapCollectable(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!OtherActor || OtherActor == this) return;
	
	AThiefCharacter* Target = Cast<AThiefCharacter>(OtherActor);
	if (!IsValid(Target)) return;
	
	
	
	if (Target->IsLocallyControlled())
	{
		AThiefPlayerController* TargetPC = Cast<AThiefPlayerController>(Target->GetController());
		TargetPC->ItemOnGroundTabWidget->RemoveItemBar(this);
	}
}



// Sets default values
AInteractableActor::AInteractableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	
	USceneComponent* SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(SceneRoot);

	HighlightCollision = CreateDefaultSubobject<USphereComponent>(TEXT("HighlightCollision"));
	HighlightCollision->SetupAttachment(SceneRoot);
	HighlightCollision->SetSphereRadius(750.0f);
	HighlightCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	HighlightCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	HighlightCollision->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);
	HighlightCollision->SetGenerateOverlapEvents(true);
	HighlightCollision->OnComponentBeginOverlap.AddDynamic(
		this,
		&AInteractableActor::OnBeginOverlapHighlight
	);
	HighlightCollision->OnComponentEndOverlap.AddDynamic(
		this,
		&AInteractableActor::OnEndOverlapHighlight
	);
	
	CollectableCollision = CreateDefaultSubobject<USphereComponent>(TEXT("CollectableCollision"));
	CollectableCollision->SetupAttachment(SceneRoot);
	CollectableCollision->SetSphereRadius(150.0f);
	CollectableCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollectableCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollectableCollision->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);
	CollectableCollision->SetGenerateOverlapEvents(true);
	CollectableCollision->OnComponentBeginOverlap.AddDynamic(
		this,
		&AInteractableActor::OnBeginOverlapCollectable
	);
	CollectableCollision->OnComponentEndOverlap.AddDynamic(
		this,
		&AInteractableActor::OnEndOverlapCollectable
	);
}

// Called when the game starts or when spawned
void AInteractableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteractableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractableActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AInteractableActor, ReplicatedItemMesh);
	DOREPLIFETIME(AInteractableActor, ItemData);
}

void AInteractableActor::OnRep_ItemMesh()
{
	if (IsValid(StaticMesh))
	{
		StaticMesh->SetStaticMesh(ReplicatedItemMesh);
		StaticMesh->SetRelativeScale3D(FVector(ReplicatedScale, ReplicatedScale, ReplicatedScale));
	}
}

void AInteractableActor::SetItemMesh(UStaticMesh* NewMesh, const float Scale)
{
	if (!HasAuthority()) return;
	
	ReplicatedItemMesh = NewMesh;
	ReplicatedScale = Scale;
	OnRep_ItemMesh();
}

void AInteractableActor::Interact(AThiefCharacter* Target)
{
	if (!HasAuthority()) return;
	
	Target->Multicast_RemoveOnGroundItem(this);
	Destroy();
}

bool AInteractableActor::IsCanCollect(AThiefCharacter* Target) const
{
	if (!IsValid(Target) || !IsValid(CollectableCollision)) return false;

	return CollectableCollision->IsOverlappingActor(Target);
}

