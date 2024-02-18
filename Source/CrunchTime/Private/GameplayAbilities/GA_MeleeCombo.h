// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilities/GA_AbilityBase.h"
#include "GA_MeleeCombo.generated.h"

/**
 * 
 */
UCLASS()
class UGA_MeleeCombo : public UGA_AbilityBase
{
public:
	UGA_MeleeCombo();
	static FGameplayTag GetComboChangeTag();
	
private:
	GENERATED_BODY()
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	UPROPERTY(EditDefaultsOnly, Category = "Anim")
	UAnimMontage* ComboMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Anim")
	TMap<FName, TSubclassOf<UGameplayEffect>> DamageEffectMap;
	
	UPROPERTY(EditDefaultsOnly, Category = "Anim")
	TSubclassOf<UGameplayEffect> DefaultDamageEffect;

	UFUNCTION()
	void HandleComboEvent(FGameplayEventData Payload);

	UFUNCTION()
	void HandleDamageEvent(FGameplayEventData Payload);

	UFUNCTION()
	void TryCommitCombo(float TimeWaited);

	FName NextComboName;
	
	bool bComboCommitted;

	void SetupWaitInputTask();

	TSubclassOf<UGameplayEffect> GetDamageEffectForCombo(FName ComboName) const;
};
