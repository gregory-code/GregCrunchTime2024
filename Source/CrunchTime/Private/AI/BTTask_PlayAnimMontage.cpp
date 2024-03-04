// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_PlayAnimMontage.h"

#include "AIController.h"

#include "BehaviorTree/BlackboardComponent.h"

#include "Character/CCharacterBase.h"

UBTTask_PlayAnimMontage::UBTTask_PlayAnimMontage()
{
	//needed for it to be ticking.
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_PlayAnimMontage::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACCharacterBase* Character = OwnerComp.GetAIOwner()->GetPawn<ACCharacterBase>();
	if (Character)
	{
		float MontageLength = Character->PlayAnimMontage(MontageToPlay);

		Character->ClientPlayAnimMontage(MontageToPlay);
		
		PlayMontageNodeData* Data = GetSpecialNodeMemory<PlayMontageNodeData>(NodeMemory);

		if (Data)
		{
			Data->MontagePlayed = MontageToPlay;
			Data->MontageTimeLeft = MontageLength;
		}

		return EBTNodeResult::InProgress;
	}

	return EBTNodeResult::Failed;
}

void UBTTask_PlayAnimMontage::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	PlayMontageNodeData* Data = GetSpecialNodeMemory<PlayMontageNodeData>(NodeMemory);
	if (Data)
	{
		Data->MontageTimeLeft -= DeltaSeconds;
		if (Data->MontageTimeLeft <= 0)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}
}

uint16 UBTTask_PlayAnimMontage::GetSpecialMemorySize() const
{
	return sizeof(PlayMontageNodeData);
}
