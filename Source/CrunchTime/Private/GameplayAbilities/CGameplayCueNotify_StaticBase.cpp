// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilities/CGameplayCueNotify_StaticBase.h"
#include "GameplayAbilities/CGameplayCueInterface.h"

void UCGameplayCueNotify_StaticBase::PlayHitReaction(AActor* Target)
{
	if (ICGameplayCueInterface* Interafce = Cast<ICGameplayCueInterface>(Target))
	{
		Interafce->PlayHitReaction();
	}
}

void UCGameplayCueNotify_StaticBase::PlayStunAnim(AActor* Target)
{
	if (ICGameplayCueInterface* Interafce = Cast<ICGameplayCueInterface>(Target))
	{
		Interafce->StartStunAnim();
	}
}

void UCGameplayCueNotify_StaticBase::StopStunAnim(AActor* Target)
{
	if (ICGameplayCueInterface* Interafce = Cast<ICGameplayCueInterface>(Target))
	{
		Interafce->StopStunAnim();
	}
}
