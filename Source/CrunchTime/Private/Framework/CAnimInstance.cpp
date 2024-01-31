// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/CAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UCAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner());
	if (OwnerCharacter)
	{
		OwnerMovemmentComp = OwnerCharacter->GetCharacterMovement();
	}
}

void UCAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);
	if (OwnerCharacter)
	{
		Speed = OwnerCharacter->GetVelocity().Length();
	}
}
