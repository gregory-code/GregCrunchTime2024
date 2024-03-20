// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilities/GA_AbilityBase.h"
#include "GameplayAbilities/CAbilityGenericTags.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Perception/AISense_Damage.h"

UGA_AbilityBase::UGA_AbilityBase()
{
	ActivationBlockedTags.AddTag(UCAbilityGenericTags::GetDeadTag());
	ActivationBlockedTags.AddTag(UCAbilityGenericTags::GetStunedTag());
}

void UGA_AbilityBase::SignalDamageStimuliEvent(FGameplayAbilityTargetDataHandle TargetHandle)
{
	TArray<AActor*> Targets = UAbilitySystemBlueprintLibrary::GetAllActorsFromTargetData(TargetHandle);

	for (AActor* Target : Targets)
	{
		UAISense_Damage::ReportDamageEvent(this, Target, GetOwningActorFromActorInfo(), 1, Target->GetActorLocation(), Target->GetActorLocation());
	}
}
