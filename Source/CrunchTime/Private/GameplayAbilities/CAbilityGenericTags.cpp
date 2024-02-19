// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilities/CAbilityGenericTags.h"

FGameplayTag UCAbilityGenericTags::GetGenericTargetAquiredTag()
{
	return FGameplayTag::RequestGameplayTag("targeting.aquired");
}
