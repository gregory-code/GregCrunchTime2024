// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "CAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class UCAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void ApplyGameplayEffect(const TSubclassOf<UGameplayEffect>& EffectToApply, int Level = 1);
	void ApplyInitialEffects();
private:
	UPROPERTY(EditDefaultsOnly, Category = "Init")
	TArray<TSubclassOf<UGameplayEffect>> InitialEffects;
};
