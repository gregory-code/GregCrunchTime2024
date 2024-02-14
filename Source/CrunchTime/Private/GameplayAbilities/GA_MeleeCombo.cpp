// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilities/GA_MeleeCombo.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"

UGA_MeleeCombo::UGA_MeleeCombo()
{
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag("ability.combo.ability"));
	BlockAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag("ability.combo.ability"));
}

FGameplayTag UGA_MeleeCombo::GetComboChangeTag()
{
	return FGameplayTag::RequestGameplayTag("ability.combo.change");
}

void UGA_MeleeCombo::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo,true,false);
		return;
	}

	UAbilityTask_PlayMontageAndWait* PlayComboMotage
		= UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this,
			NAME_None,
			ComboMontage);

	PlayComboMotage->OnBlendOut.AddDynamic(this, &UGA_MeleeCombo::K2_EndAbility);
	PlayComboMotage->OnInterrupted.AddDynamic(this, &UGA_MeleeCombo::K2_EndAbility);
	PlayComboMotage->OnCancelled.AddDynamic(this, &UGA_MeleeCombo::K2_EndAbility);
	PlayComboMotage->OnCompleted.AddDynamic(this, &UGA_MeleeCombo::K2_EndAbility);
	PlayComboMotage->ReadyForActivation();

	UAbilityTask_WaitGameplayEvent* WaitComboEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, GetComboChangeTag(), nullptr, false, false);
	WaitComboEvent->EventReceived.AddDynamic(this, &UGA_MeleeCombo::HandleComboEvent);
	WaitComboEvent->ReadyForActivation();
}

void UGA_MeleeCombo::HandleComboEvent(FGameplayEventData Payload)
{
	UE_LOG(LogTemp, Warning, TEXT("Handing Event With Tag: %s"), *Payload.EventTag.ToString());
}
