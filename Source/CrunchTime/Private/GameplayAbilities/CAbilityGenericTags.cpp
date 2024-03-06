// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilities/CAbilityGenericTags.h"

FGameplayTag UCAbilityGenericTags::GetGenericTargetAquiredTag()
{
	return FGameplayTag::RequestGameplayTag("targeting.aquired");
}

FGameplayTag UCAbilityGenericTags::GetFullHealthTag()
{
	return FGameplayTag::RequestGameplayTag("stat.fullHealth");
}

FGameplayTag UCAbilityGenericTags::GetDeadTag()
{
	return FGameplayTag::RequestGameplayTag("stat.dead");
}

FGameplayTag UCAbilityGenericTags::GetBasicAttackAcitvationTag()
{
	return FGameplayTag::RequestGameplayTag("ability.attack.activate");
}

FGameplayTag UCAbilityGenericTags::GetAimingTag()
{
	return FGameplayTag::RequestGameplayTag("stat.aiming");
}
