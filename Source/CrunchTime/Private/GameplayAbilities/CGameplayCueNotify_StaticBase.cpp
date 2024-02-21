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
