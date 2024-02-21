// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilities/GA_AbilityBase.h"
#include "GameplayAbilities/CAbilityGenericTags.h"

UGA_AbilityBase::UGA_AbilityBase()
{
	ActivationBlockedTags.AddTag(UCAbilityGenericTags::GetDeadTag());
}
