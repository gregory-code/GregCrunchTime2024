// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilities/GA_GroundBlast.h"

#include "Abilities/Tasks/AbilityTask_WaitTargetData.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

#include "GameplayAbilities/CAbilityGenericTags.h"

#include "Targeting/CTargetActor_GroundPick.h"


UGA_GroundBlast::UGA_GroundBlast()
{
	ActivationOwnedTags.AddTag(UCAbilityGenericTags::GetAimingTag());
}

void UGA_GroundBlast::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		K2_EndAbility();
		return;
	}

	if ((!CheckCost(Handle, ActorInfo)) || (!CheckCooldown(Handle, ActorInfo)))
	{
		K2_EndAbility();
		return;
	}

	UAbilityTask_PlayMontageAndWait* PlayTargetingMontageTask =
		UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, TargettingMontage);

	PlayTargetingMontageTask->OnBlendOut.AddDynamic(this, &UGA_GroundBlast::K2_EndAbility);
	PlayTargetingMontageTask->OnInterrupted.AddDynamic(this, &UGA_GroundBlast::K2_EndAbility);
	PlayTargetingMontageTask->OnCompleted.AddDynamic(this, &UGA_GroundBlast::K2_EndAbility);
	PlayTargetingMontageTask->OnCancelled.AddDynamic(this, &UGA_GroundBlast::K2_EndAbility);
	PlayTargetingMontageTask->ReadyForActivation();

	UAbilityTask_WaitTargetData* WaitTargetDataTask = UAbilityTask_WaitTargetData::WaitTargetData(this, NAME_None,
		EGameplayTargetingConfirmation::UserConfirmed, TargetActorClass);

	WaitTargetDataTask->ValidData.AddDynamic(this, &UGA_GroundBlast::TargetAquired);
	WaitTargetDataTask->Cancelled.AddDynamic(this, &UGA_GroundBlast::TargetCancelled);
	WaitTargetDataTask->ReadyForActivation();
	
	AGameplayAbilityTargetActor* SpawnedTargetActor;
	WaitTargetDataTask->BeginSpawningActor(this, TargetActorClass, SpawnedTargetActor);
	ACTargetActor_GroundPick* GroundPickActor = Cast<ACTargetActor_GroundPick>(SpawnedTargetActor);
	if (GroundPickActor)
	{
		GroundPickActor->SetTargettingRadius(TargetingRadius);
		GroundPickActor->SetTargettingRange(TargettingRange);
	}
	WaitTargetDataTask->FinishSpawningActor(this, GroundPickActor);

}

void UGA_GroundBlast::TargetAquired(const FGameplayAbilityTargetDataHandle& Data)
{
	UE_LOG(LogTemp, Warning, TEXT("Target aquired"));
	if (HasAuthorityOrPredictionKey(CurrentActorInfo, &CurrentActivationInfo))
	{
		if (!K2_CommitAbility())
		{
			K2_EndAbility();
			return;
		}

		for (TSubclassOf<UGameplayEffect>& DamageEffect : DamageEffects)
		{
			FGameplayEffectSpecHandle DamageSpec = MakeOutgoingGameplayEffectSpec(DamageEffect, GetCurrentAbilitySpec()->Level);
			ApplyGameplayEffectSpecToTarget(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), DamageSpec, Data);
		}
		SignalDamageStimuliEvent(Data);
	}

	const FHitResult* BlastLocationHitResult = Data.Get(1)->GetHitResult();
	if (BlastLocationHitResult)
	{
		ExecuteSpawnVFXCue(BlastVFX, BlastVFXSize, BlastLocationHitResult->ImpactPoint);
	}

	GetOwningComponentFromActorInfo()->GetAnimInstance()->Montage_Play(CastingMontage);

	K2_EndAbility();
}

void UGA_GroundBlast::TargetCancelled(const FGameplayAbilityTargetDataHandle& Data)
{
	K2_EndAbility();
}
