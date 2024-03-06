// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilities/GA_GroundBlast.h"

void UGA_GroundBlast::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		K2_EndAbility();
		return;
	}

	if (!K2_CommitAbility())
	{
		K2_EndAbility();
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Casting GroundBlast"));
}
