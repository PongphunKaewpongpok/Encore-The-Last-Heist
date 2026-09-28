// Fill out your copyright notice in the Description page of Project Settings.


#include "WeretigerCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"

FString AWeretigerCharacter::GetForm()
{
	return Form;
}

void AWeretigerCharacter::SwitchForm()
{
	GetCharacterMovement()->MaxWalkSpeed = 0.0f;
	
	if (Form.Equals("Human"))
	{
		Form = TEXT("Tiger");
	}
	else
	{
		Form = TEXT("Human");
	}
}
