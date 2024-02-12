// Fill out your copyright notice in the Description page of Project Settings.
#include "GameplayAbilities/CAbilitySystemComponent.h"

void UCAbilitySystemComponent::ApplyGameplayEffect_Implementation(TSubclassOf<UGameplayEffect> EffectToApply, int Level)
{
	FGameplayEffectSpecHandle specHandle = MakeOutgoingSpec(EffectToApply, Level, MakeEffectContext());
	ApplyGameplayEffectSpecToSelf(*specHandle.Data.Get());
}

bool UCAbilitySystemComponent::ApplyGameplayEffect_Validate(TSubclassOf<UGameplayEffect> EffectToApply, int Level)
{
	return true;
}

void UCAbilitySystemComponent::ApplyInitialEffects()
{
	for (const TSubclassOf<UGameplayEffect>& Effect : InitialEffects)
	{
		ApplyGameplayEffect(Effect, 1);


	}
}
