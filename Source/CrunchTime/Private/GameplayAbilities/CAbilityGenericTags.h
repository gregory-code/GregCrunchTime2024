// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayTagContainer.h"
#include "CAbilityGenericTags.generated.h"

/**
 * 
 */
UCLASS()
class UCAbilityGenericTags : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	static FGameplayTag GetGenericTargetAquiredTag();
	static FGameplayTag GetFullHealthTag();
	static FGameplayTag GetDeadTag();
	static FGameplayTag GetBasicAttackAcitvationTag();
	static FGameplayTag GetAimingTag();
};
