// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/CPlayerController.h"
#include "Player/CPlayerCharacter.h"
#include "Widgets/GameplayUI.h"

void ACPlayerController::OnPossess(APawn* NewPawn)
{
	Super::OnPossess(NewPawn);
	PostPossessionSetup(NewPawn);
}

void ACPlayerController::AcknowledgePossession(APawn* NewPawn)
{
	Super::AcknowledgePossession(NewPawn);
	PostPossessionSetup(NewPawn);
}

void ACPlayerController::PostPossessionSetup(APawn* NewPawn)
{
	PlayerCharacter = Cast<ACPlayerCharacter>(NewPawn);
	if (!PlayerCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("Needs CPlayerCharacter as Pawn"));
		return;
	}

	PlayerCharacter->SetupAbilitySystemComponent();
	SpawnGameplayUI();
	if (IsLocalPlayerController())
	{
		PlayerCharacter->InitAbilityAndAttributes();
	}
}

void ACPlayerController::SpawnGameplayUI()
{
	if (!PlayerCharacter)
	{
		return;
	}

	if (!GameplayUIClass)
	{
		UE_LOG(LogTemp, Error, TEXT("%s,  missing Gameplay UI class, Can't Spawn Gameplay UI"), *GetName());
		return;
	}
	//return true, if this player controller represent a human player sitting infront of this manchine.
	if (!IsLocalPlayerController())
	{
		return;
	}

	GameplayUI = CreateWidget<UGameplayUI>(this, GameplayUIClass);
	if (GameplayUI)
	{
		GameplayUI->AddToViewport();
	}
}
