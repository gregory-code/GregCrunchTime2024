// Fill out your copyright notice in the Description page of Project Settings.


#include "Targeting/AnimNotifyState_Targeting.h"
#include "Targeting/TargetingBoxComponent.h"

void UAnimNotifyState_Targeting::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	const AActor* OwnerActor = MeshComp->GetOwner();
	TargetingBoxComponent = OwnerActor->GetComponentByClass<UTargetingBoxComponent>();
	if (TargetingBoxComponent)
	{
		TargetingBoxComponent->AttachToComponent(MeshComp, FAttachmentTransformRules::SnapToTargetIncludingScale, AttachSocket);
		TargetingBoxComponent->StartDetection();
	}
}

void UAnimNotifyState_Targeting::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);
	if (TargetingBoxComponent)
	{
		TargetingBoxComponent->DoTargetCheck();
	}
}

void UAnimNotifyState_Targeting::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (TargetingBoxComponent)
	{
		TargetingBoxComponent->EndDetection();
	}
}
