// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayEffectTypes.h"
#include "AbilityGuage.generated.h"

struct FGameplayAbilitySpec;
class UGA_AbilityBase;
class UGameplayAbility;
/**
 * 
 */
UCLASS()
class UAbilityGuage : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetupOwingAbilityCDO(const UGA_AbilityBase* OwningAbilityCDO);
	void SubscribeAbilityDelegates();
private:
	void AbilityCommited(UGameplayAbility* Ability);

	UPROPERTY(meta = (BindWidget))
	class UImage* IconImage;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CooldownDurationText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ManaCostText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CooldownCounterText;

	UPROPERTY(EditDefaultsOnly, Category = "Cooldown")
	FName CooldownPercentMaterialParamName = "Percent";
	
	UPROPERTY(EditDefaultsOnly, Category = "Cooldown")
	FName NotEnoughManaShadeAlphaMaterialParamName = "NotEnoughManaShadeAlpha";

	UPROPERTY(EditDefaultsOnly, Category = "Icon")
	FName IconTextureMaterialParamName = "Icon";

	const class UGA_AbilityBase* AbilityCDO;
	UMaterialInstanceDynamic* IconMat;

	UPROPERTY(EditDefaultsOnly, Category = "Cooldown")
	float CooldownTickInterval = 1.f / 24.f;

	FTimerHandle CooldownTickTimerHandle;

	void TickCooldown();

	void CooldownFinished();

	float CooldownDuration = 0;
	float CooldownTimeRemaining = 0;

	void ManaUpdated(const FOnAttributeChangeData& ChangeData);
};
