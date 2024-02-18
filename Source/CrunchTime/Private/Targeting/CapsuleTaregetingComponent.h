// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "GameplayTagContainer.h"
#include "CapsuleTaregetingComponent.generated.h"

/**
 * 
 */
UCLASS()
class UCapsuleTaregetingComponent : public UCapsuleComponent
{
	GENERATED_BODY()
	
public:
	UCapsuleTaregetingComponent();
	void SetDetectionEnabled(bool bCollisionEnabled);
	void StartTargetting();
	void StopTargetting();
	static FGameplayTag GetTargetAquiredEventTag();
private:
	UFUNCTION()
	void Overlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void DoStaticDetect();

	TSet<AActor*> OverlappedActors;

	void TargetDetected(AActor* TargetActor);
};
