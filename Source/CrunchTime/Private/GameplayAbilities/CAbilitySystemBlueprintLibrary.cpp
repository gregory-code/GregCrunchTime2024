// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilities/CAbilitySystemBlueprintLibrary.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystemComponent.h"

float UCAbilitySystemBlueprintLibrary::GetAbilityStaticCooldownDuration(const UGameplayAbility* AbilityCDO)
{
	float Duration = 0;
	UGameplayEffect* CooldownEffect = AbilityCDO->GetCooldownGameplayEffect();
	if (CooldownEffect)
	{
		CooldownEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(1, Duration);
	}
	return Duration;
}

float UCAbilitySystemBlueprintLibrary::GetAbilityStaticManaCost(const UGameplayAbility* AbilityCDO)
{
	float Cost = 0;
	UGameplayEffect* CostEffect = AbilityCDO->GetCostGameplayEffect();
	if (CostEffect && CostEffect->Modifiers.Num() > 0)
	{
		CostEffect->Modifiers[0].ModifierMagnitude.GetStaticMagnitudeIfPossible(1, Cost);
	}

	return -Cost;
}

float UCAbilitySystemBlueprintLibrary::GetActiveAbilityManaCost(const UAbilitySystemComponent* ASC, const UGameplayAbility* AbilityCDO)
{
	FGameplayAbilitySpec* ActiveAbilitySpec = ASC->FindAbilitySpecFromClass(AbilityCDO->GetClass());
	if (!ActiveAbilitySpec)
	{
		return GetAbilityStaticManaCost(AbilityCDO);
	}
	float Cost = 0;
	UGameplayEffect* CostEffect = AbilityCDO->GetCostGameplayEffect();
	if (CostEffect&& CostEffect->Modifiers.Num()>0)
	{
		FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(CostEffect->GetClass(),  ActiveAbilitySpec->Level, ASC->MakeEffectContext());
		CostEffect->Modifiers[0].ModifierMagnitude.AttemptCalculateMagnitude(*SpecHandle.Data.Get(), Cost);
	}

	return abs(Cost);
}

