// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilities/GA_AbilityBase.h"
#include "GA_GroundBlast.generated.h"

/**
 * 
 */
UCLASS()
class UGA_GroundBlast : public UGA_AbilityBase
{
public:
	UGA_GroundBlast();
private:
	GENERATED_BODY()
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TArray<TSubclassOf<UGameplayEffect>> DamageEffects;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* TargettingMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* CastingMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Targeting")
	TSubclassOf<class ACTargetActor_GroundPick> TargetActorClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Targeting")
	float TargetingRadius = 200.f;

	UPROPERTY(EditDefaultsOnly, Category = "Targeting")
	float TargettingRange = 1200.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	UParticleSystem* BlastVFX;

	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	float BlastVFXSize = 10.f;

	UFUNCTION()
	void TargetAquired(const FGameplayAbilityTargetDataHandle& Data);
	
	UFUNCTION()
	void TargetCancelled(const FGameplayAbilityTargetDataHandle& Data);
};
