// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_DoBasicAttack.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayAbilities/CAbilityGenericTags.h"
#include "AIController.h"

EBTNodeResult::Type UBTTask_DoBasicAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AAIController* AIC = OwnerComp.GetAIOwner())
	{
		if (APawn* OwningPawn = AIC->GetPawn())
		{
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwningPawn, UCAbilityGenericTags::GetBasicAttackAcitvationTag(), FGameplayEventData());
		}
	}

	return EBTNodeResult::Succeeded;
}
