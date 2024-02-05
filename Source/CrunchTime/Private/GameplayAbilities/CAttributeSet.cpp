// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilities/CAttributeSet.h"
#include "Net/UnrealNetwork.h"

void UCAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UCAttributeSet, Health, COND_None);
	DOREPLIFETIME_CONDITION(UCAttributeSet, MaxHealth, COND_None);
}
