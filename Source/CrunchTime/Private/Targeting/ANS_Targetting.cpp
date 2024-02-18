// Fill out your copyright notice in the Description page of Project Settings.


#include "Targeting/ANS_Targetting.h"
#include "Targeting/CapsuleTaregetingComponent.h"

void UANS_Targetting::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	if (!MeshComp->GetOwner())
		return;

	UCapsuleTaregetingComponent* damageComp = MeshComp->GetOwner()->GetComponentByClass<UCapsuleTaregetingComponent>();
	if (!damageComp)
		return;

	damageComp->AttachToComponent(MeshComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale, DamageAttachSlotName);
	damageComp->StartTargetting();
}

void UANS_Targetting::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (!MeshComp->GetOwner())
		return;

	UCapsuleTaregetingComponent* damageComp = MeshComp->GetOwner()->GetComponentByClass<UCapsuleTaregetingComponent>();
	if (!damageComp)
		return;

	damageComp->StopTargetting();
}

FString UANS_Targetting::GetNotifyName_Implementation() const
{
	return DamageAttachSlotName.ToString();
}
