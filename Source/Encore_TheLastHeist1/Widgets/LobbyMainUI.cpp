// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/LobbyMainUI.h"

#include "LobbyDisplayNameBar.h"
#include "Components/TextBlock.h"
#include "LobbyGameState.h"
#include "LobbyPlayerState.h"
#include "ThiefPlayerState.h"
#include "TimerManager.h"
#include "Components/ScrollBox.h"


void ULobbyMainUI::NativeConstruct()
{
	Super::NativeConstruct();

	ALobbyGameState* GS = GetWorld()->GetGameState<ALobbyGameState>();
	if (!IsValid(GS)) return;

	for (APlayerState* PS : GS->PlayerArray)
	{
		if (ALobbyPlayerState* LobbyPS = Cast<ALobbyPlayerState>(PS))
		{
			BindSinglePlayerState(LobbyPS);
		}
	}

	GS->OnLobbyPlayerStateAdded.AddUObject(
		this,
		&ULobbyMainUI::BindSinglePlayerState
	);
	
	RefreshDisplayNameTab();
}

void ULobbyMainUI::HandleDataChanged()
{
	RefreshDisplayNameTab();
}

void ULobbyMainUI::BindSinglePlayerState(ALobbyPlayerState* LobbyPS)
{
	if (!IsValid(LobbyPS)) return;

	LobbyPS->OnDataChanged.AddUniqueDynamic(
		this,
		&ULobbyMainUI::HandleDataChanged
	);

	RefreshDisplayNameTab();
}

void ULobbyMainUI::RefreshDisplayNameTab()
{
	if (!IsValid(DisplayNameListTab) || !IsValid(LobbyDisplayNameBarClass)) return;

	ALobbyGameState* GS = GetWorld()->GetGameState<ALobbyGameState>();
	if (!IsValid(GS)) return;

	DisplayNameListTab->ClearChildren();

	TArray<ALobbyPlayerState*> Players = GS->GetAllLobbyPlayerStates();
	
	for (ALobbyPlayerState* PS : Players)
	{
		if (!IsValid(PS)) continue;

		ULobbyDisplayNameBar* LobbyDisplayNameBar =
			CreateWidget<ULobbyDisplayNameBar>(
				GetOwningPlayer(),
				LobbyDisplayNameBarClass
			);

		if (!IsValid(LobbyDisplayNameBar)) continue;

		LobbyDisplayNameBar->Setup(PS);
		
		DisplayNameListTab->AddChild(LobbyDisplayNameBar);
	}
}

