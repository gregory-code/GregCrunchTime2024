// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilities/GA_MeleeCombo.h"

#include "Animation/AnimInstance.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_WaitInputPress.h"

#include "Components/SkeletalMeshComponent.h"

#include "GameplayTagsManager.h"

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

	SetupWaitInputTask();
}

void UGA_MeleeCombo::HandleComboEvent(FGameplayEventData Payload)
{
	FGameplayTag ComboTag = Payload.EventTag;
	if (ComboTag == FGameplayTag::RequestGameplayTag("ability.combo.change.end"))
	{
		NextComboName = NAME_None;
		bComboCommitted = false;
		return;
	}

	TArray<FName> ComboNames;
	UGameplayTagsManager::Get().SplitGameplayTagFName(ComboTag, ComboNames);	
	NextComboName = ComboNames.Last();
}

void UGA_MeleeCombo::TryCommitCombo(float TimeWaited)
{
	SetupWaitInputTask();

	if (bComboCommitted)
		return;

	if (NextComboName == NAME_None)
	{
		return;
	}

	USkeletalMeshComponent* OwnerMesh = GetOwningComponentFromActorInfo();
	if (!OwnerMesh)
		return;

	UAnimInstance* OwnerAnimInst = OwnerMesh->GetAnimInstance();
	if (!OwnerAnimInst)
		return;

	OwnerAnimInst->Montage_SetNextSection(OwnerAnimInst->Montage_GetCurrentSection(ComboMontage), NextComboName, ComboMontage);
	bComboCommitted = true;
}

void UGA_MeleeCombo::SetupWaitInputTask()
{
	UAbilityTask_WaitInputPress* WaitInputPress = UAbilityTask_WaitInputPress::WaitInputPress(this);
	WaitInputPress->OnPress.AddDynamic(this, &UGA_MeleeCombo::TryCommitCombo);
	WaitInputPress->ReadyForActivation();
}
