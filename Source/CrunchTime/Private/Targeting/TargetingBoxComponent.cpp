// Fill out your copyright notice in the Description page of Project Settings.


#include "Targeting/TargetingBoxComponent.h"

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
			const AActor* OverlappedActor = result.GetActor();
			if (AlreadyDetectedActors.Contains(OverlappedActor))
			{
				continue;
			}

			AlreadyDetectedActors.Add(OverlappedActor);
			TargetFound(OverlappedActor);
		}
	}
}

void UTargetingBoxComponent::EndDetection()
{
	SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AlreadyDetectedActors.Empty();
}

void UTargetingBoxComponent::TargetFound(const AActor* OverlappedActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Found Target: %s"), *OverlappedActor->GetName());
}
