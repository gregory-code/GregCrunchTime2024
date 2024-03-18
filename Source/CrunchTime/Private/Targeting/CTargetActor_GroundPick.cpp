// Fill out your copyright notice in the Description page of Project Settings.


#include "Targeting/CTargetActor_GroundPick.h"
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
