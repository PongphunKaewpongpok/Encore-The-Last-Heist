// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableActor.h"

#include "Components/SphereComponent.h"
#include "Components/PrimitiveComponent.h"
#include "ThiefCharacter.h"
#include "Net/UnrealNetwork.h"
#include "Components/StaticMeshComponent.h"

void AInteractableActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
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

void AInteractableActor::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
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

// Sets default values
AInteractableActor::AInteractableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SetRootComponent(StaticMesh);

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetupAttachment(StaticMesh);
	SphereCollision->SetSphereRadius(750.0f);
	SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereCollision->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);
	SphereCollision->SetGenerateOverlapEvents(true);
	SphereCollision->OnComponentBeginOverlap.AddDynamic(
		this,
		&AInteractableActor::OnBeginOverlap
	);
	SphereCollision->OnComponentEndOverlap.AddDynamic(
		this,
		&AInteractableActor::OnEndOverlap
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
}

void AInteractableActor::OnRep_ItemMesh()
{
	if (IsValid(StaticMesh))
	{
		StaticMesh->SetStaticMesh(ReplicatedItemMesh);
	}
}

void AInteractableActor::SetItemMesh(UStaticMesh* NewMesh)
{
	if (!HasAuthority()) return;
	ReplicatedItemMesh = NewMesh;

	OnRep_ItemMesh();
}

