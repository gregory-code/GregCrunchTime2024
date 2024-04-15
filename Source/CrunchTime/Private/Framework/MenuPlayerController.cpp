// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/MenuPlayerController.h"
#include "Blueprint/UserWidget.h"

void AMenuPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (IsLocalPlayerController() && MenuToSpawn)
	{
		UUserWidget* Widget = CreateWidget<UUserWidget>(this, MenuToSpawn);
		Widget->AddToViewport();
	}
}
