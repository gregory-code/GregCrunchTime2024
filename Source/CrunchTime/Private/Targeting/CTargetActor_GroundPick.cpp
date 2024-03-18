// Fill out your copyright notice in the Description page of Project Settings.


#include "Targeting/CTargetActor_GroundPick.h"

#include "AbilitySystemBlueprintLibrary.h"

#include "Components/SphereComponent.h"
#include "Components/DecalComponent.h"

ACTargetActor_GroundPick::ACTargetActor_GroundPick()
{
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);

	TargetSphere = CreateDefaultSubobject<USphereComponent>("TargetSphere");
	TargetSphere->SetupAttachment(GetRootComponent());

	TargetSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TargetSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	TargetSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	TargetDecal = CreateDefaultSubobject<UDecalComponent>("TargetDecal");
	TargetDecal->SetupAttachment(GetRootComponent());

	PrimaryActorTick.bCanEverTick = true;
}

void ACTargetActor_GroundPick::SetTargettingRadius(float TargettingRadius)
{
	TargetSphere->SetSphereRadius(TargettingRadius);
	TargetDecal->DecalSize = FVector{TargettingRadius};
}

void ACTargetActor_GroundPick::SetTargettingRange(float NewTargettingRange)
{
	TargettingRange = NewTargettingRange;
}

void ACTargetActor_GroundPick::Tick(float DeltaSecond)
{
	Super::Tick(DeltaSecond);
	FHitResult PlayerView = GetPlayerView();
	if (PlayerView.bBlockingHit)
	{
		SetActorLocation(PlayerView.ImpactPoint);
	}
}

bool ACTargetActor_GroundPick::IsConfirmTargetingAllowed()
{
	return GetPlayerView().bBlockingHit;
}

void ACTargetActor_GroundPick::ConfirmTargetingAndContinue()
{
	//set is having all unique elements.
	TSet<AActor*> CurrentActorsInRange;
	TargetSphere->GetOverlappingActors(CurrentActorsInRange, APawn::StaticClass());

	TArray<AActor*> Targets;
	for (AActor* ActorInRange : CurrentActorsInRange)
	{
		if(ActorInRange != PrimaryPC->GetPawn())
			Targets.Add(ActorInRange);
	}

	FGameplayAbilityTargetDataHandle TargetDataHandle = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActorArray(Targets, false);
	TargetDataReadyDelegate.Broadcast(TargetDataHandle);
}

FHitResult ACTargetActor_GroundPick::GetPlayerView() const
{
	FHitResult HitResult;
	if (PrimaryPC)
	{
		FVector ViewLoc;
		FRotator ViewRot;

		PrimaryPC->GetPlayerViewPoint(ViewLoc, ViewRot);
		
		FVector TraceEnd = ViewLoc + ViewRot.Vector() * TargettingRange;

		GetWorld()->LineTraceSingleByChannel(HitResult, ViewLoc, TraceEnd, ECC_Visibility);
		if (!HitResult.bBlockingHit)
		{
			GetWorld()->LineTraceSingleByChannel(HitResult, TraceEnd, TraceEnd + FVector::DownVector * TargettingRange, ECC_Visibility);
		}
	}

	return HitResult;
}
