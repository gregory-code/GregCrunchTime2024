// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CCharacterBase.h"
#include "GameplayAbilities/CAbilitySystemComponent.h"
#include "GameplayAbilities/CAttributeSet.h"

// Sets default values
ACCharacterBase::ACCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UCAbilitySystemComponent>("Ability System Component");
	AbilitySystemComponent->SetIsReplicated(true); // replicate means it is synced with the server.
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UCAttributeSet>("Attribute Set");
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

UAbilitySystemComponent* ACCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

