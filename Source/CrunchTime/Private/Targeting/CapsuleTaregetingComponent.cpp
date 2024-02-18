// Fill out your copyright notice in the Description page of Project Settings.


#include "Targeting/CapsuleTaregetingComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

UCapsuleTaregetingComponent::UCapsuleTaregetingComponent()
{
	OnComponentBeginOverlap.AddDynamic(this, &UCapsuleTaregetingComponent::Overlapped);
	SetDetectionEnabled(false);
}

void UCapsuleTaregetingComponent::SetDetectionEnabled(bool bCollisionEnabled)
{
	if (bCollisionEnabled)
	{
		SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void UCapsuleTaregetingComponent::StartTargetting()
{
	OverlappedActors.Empty();
	SetDetectionEnabled(true);
	DoStaticDetect();
}

void UCapsuleTaregetingComponent::StopTargetting()
{
	DoStaticDetect();
	SetDetectionEnabled(false);
	OverlappedActors.Empty();
}

FGameplayTag UCapsuleTaregetingComponent::GetTargetAquiredEventTag()
{
	return FGameplayTag::RequestGameplayTag("targeting.aquired");
}

void UCapsuleTaregetingComponent::Overlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	TargetDetected(OtherActor);
}

void UCapsuleTaregetingComponent::DoStaticDetect()
{
	TSet<AActor*> CurrentlyOverlappedActors;

	TArray<FOverlapResult> Results;

	FCollisionShape CollisionShape;
	CollisionShape.SetCapsule(GetScaledCapsuleRadius(), GetScaledCapsuleHalfHeight());
	GetWorld()->OverlapMultiByChannel(Results, GetComponentLocation(), GetComponentQuat(), ECC_Pawn,  CollisionShape);

	for (const FOverlapResult& Result: Results)
	{
		TargetDetected(Result.GetActor());
	}
}

void UCapsuleTaregetingComponent::TargetDetected(AActor* TargetActor)
{
	if (!GetOwner()->HasAuthority())
	{
		return;
	}

	if (TargetActor == GetOwner())
	{
		return;
	}

	if (OverlappedActors.Contains(TargetActor))
		return;

	OverlappedActors.Add(TargetActor);
	FGameplayEventData Data;

	FGameplayAbilityTargetData_ActorArray* TargetArray = new FGameplayAbilityTargetData_ActorArray();
	TargetArray->SetActors(TArray<TWeakObjectPtr<AActor>>{TargetActor});
	Data.TargetData.Add(TargetArray);

	FHitResult HitResult;
	HitResult.ImpactPoint = GetComponentLocation();
	Data.ContextHandle = new FGameplayEffectContext();
	Data.ContextHandle.AddHitResult(HitResult);

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwner(), GetTargetAquiredEventTag(), Data);
}
