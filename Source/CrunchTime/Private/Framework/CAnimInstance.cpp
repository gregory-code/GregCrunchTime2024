// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/CAnimInstance.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameplayAbilities/CAbilityGenericTags.h"

bool UCAnimInstance::ShouldDoUpperBody() const
{
	return IsMoving() || IsJumping() || GetIsAiming();
}

void UCAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner());
	if (OwnerCharacter)
	{
		OwnerMovemmentComp = OwnerCharacter->GetCharacterMovement();
		PrevRot = OwnerCharacter->GetActorRotation();
		UAbilitySystemComponent* OwnerASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TryGetPawnOwner());
		if (OwnerASC)
		{
			OwnerASC->RegisterGameplayTagEvent(UCAbilityGenericTags::GetAimingTag()).AddUObject(this, &UCAnimInstance::AimingTagChanged);
		}
	}

	if (StartMontage)
	{
		Montage_Play(StartMontage);

	}


}

void UCAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);
	if (OwnerCharacter && OwnerMovemmentComp)
	{
		Speed = OwnerCharacter->GetVelocity().Length();
		bIsJumping = OwnerMovemmentComp->IsFalling();

		FRotator characterRot = OwnerCharacter->GetActorRotation();
		FRotator lookRot = OwnerCharacter->GetViewRotation();

		LookOffset = (lookRot - characterRot).GetNormalized();
	
		FRotator RotDelta = (characterRot - PrevRot).GetNormalized();
		PrevRot = characterRot;

		YawSpeed = FMath::FInterpTo(YawSpeed, RotDelta.Yaw/DeltaSeconds, DeltaSeconds, 10.f);

		FVector Velocity = OwnerCharacter->GetVelocity();

		FVector LookDir = lookRot.Vector();
		LookDir.Z = 0;
		LookDir.Normalize();
		FwdSpeed = Velocity.Dot(LookDir);
		RightSpeed = -Velocity.Dot(LookDir.Cross(FVector::UpVector));
	}
}

void UCAnimInstance::AimingTagChanged(const FGameplayTag TagChanged, int32 NewStackCount)
{
	bIsAiming = NewStackCount != 0;
}
