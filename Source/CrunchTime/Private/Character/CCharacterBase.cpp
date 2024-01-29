// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CCharacterBase.h"

// Sets default values
ACCharacterBase::ACCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

