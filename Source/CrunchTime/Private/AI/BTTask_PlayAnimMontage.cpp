// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_PlayAnimMontage.h"

#include "AIController.h"

#include "BehaviorTree/BlackboardComponent.h"

#include "Character/CCharacterBase.h"

EBTNodeResult::Type UBTTask_PlayAnimMontage::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACCharacterBase* Character = OwnerComp.GetAIOwner()->GetPawn<ACCharacterBase>();
	if (Character)
	{
		Character->PlayAnimMontage(MontageToPlay);

		Character->ClientPlayAnimMontage(MontageToPlay);
		
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
