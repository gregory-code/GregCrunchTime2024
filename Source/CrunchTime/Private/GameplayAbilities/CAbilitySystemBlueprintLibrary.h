// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "CAbilitySystemBlueprintLibrary.generated.h"

/**
 * 
 */
UCLASS()
class UCAbilitySystemBlueprintLibrary : public UAbilitySystemBlueprintLibrary
{
	GENERATED_BODY()
public:
	static float GetAbilityStaticCooldownDuration(const UGameplayAbility* AbilityCDO);
	static float GetAbilityStaticManaCost(const UGameplayAbility* AbilityCDO);
};
