// Fill out your copyright notice in the Description page of Project Settings.


#include "Targeting/TargetingBoxComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayAbilities/CAbilityGenericTags.h"

void UTargetingBoxComponent::StartDetection()
{
	AlreadyDetectedActors.Empty();
	SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void UTargetingBoxComponent::DoTargetCheck()
{
	TArray<FOverlapResult> OutResult;
	FCollisionShape DetecteionShape;
	const FVector Extend = GetScaledBoxExtent();
	DetecteionShape.SetBox(FVector3f{(float)Extend.X, (float)Extend.Y, (float)Extend.Z});

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());

	if (GetWorld()->OverlapMultiByChannel(OutResult,
																	GetComponentLocation(),
																	GetComponentQuat(),
																	ECC_Pawn,
																	DetecteionShape,
																	QueryParams))
	{
		for (const FOverlapResult& result : OutResult)
		{
			AActor* OverlappedActor = result.GetActor();
			if(result.GetComponent() == OverlappedActor->GetRootComponent())
				TargetFound(OverlappedActor);
		}
	}

	//DrawDebugBox(GetWorld(), GetComponentLocation(), GetScaledBoxExtent(), FColor::Red, false, 1.f);
}

void UTargetingBoxComponent::EndDetection()
{
	SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AlreadyDetectedActors.Empty();
}

void UTargetingBoxComponent::TargetFound(AActor* OverlappedActor)
{
	if (!GetOwner()->HasAuthority())
	{
		return;
	}

	if (OverlappedActor == GetOwner())
	{
		return;
	}

	if (AlreadyDetectedActors.Contains(OverlappedActor))
	{
		return;
	}

	AlreadyDetectedActors.Add(OverlappedActor);
	FGameplayEventData Data;
	Data.TargetData = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActor(OverlappedActor);
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwner(), UCAbilityGenericTags::GetGenericTargetAquiredTag(), Data);
}
