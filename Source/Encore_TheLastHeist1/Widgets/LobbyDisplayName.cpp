// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/LobbyDisplayName.h"

#include "LobbyPlayerState.h"
#include "Components/TextBlock.h"


void ULobbyDisplayName::NativeConstruct()
{
	Super::NativeConstruct();
	
	BindPlayerState();
}

void ULobbyDisplayName::HandleDataChanged()
{
	APlayerController* PC = GetOwningPlayer();
	if (!IsValid(PC)) return;

	ALobbyPlayerState* PS = PC->GetPlayerState<ALobbyPlayerState>();
	if (!IsValid(PS)) return;
	
	SetDisplayName(PS->DisplayName);
}

void ULobbyDisplayName::BindPlayerState()
{
	APlayerController* PC = GetOwningPlayer();
	if (!IsValid(PC)) return;

	ALobbyPlayerState* PS = PC->GetPlayerState<ALobbyPlayerState>();
	if (!IsValid(PS)) return;
	
	PS->OnDataChanged.AddUniqueDynamic(
		this,
		&ULobbyDisplayName::HandleDataChanged
	);
	
	DisplayNameText->SetText(PS->DisplayName);
}

void ULobbyDisplayName::SetDisplayName(const FText& NewDisplayName) const
{
	if (!DisplayNameText) return;
	
	DisplayNameText->SetText(NewDisplayName);
}
