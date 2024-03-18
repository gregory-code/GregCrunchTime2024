// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "CTargetActor_GroundPick.generated.h"

/**
 * 
 */
UCLASS()
class ACTargetActor_GroundPick : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()
public:
	ACTargetActor_GroundPick();

	void SetTargettingRadius(float TargettingRadius);
	
	void SetTargettingRange(float NewTargettingRange);

	void Tick(float DeltaSecond) override;

	virtual bool IsConfirmTargetingAllowed() override;

	virtual void ConfirmTargetingAndContinue() override;

private:
	UPROPERTY(VisibleDefaultsOnly, Category = "Targetting")
	class USphereComponent* TargetSphere;
	
	UPROPERTY(VisibleDefaultsOnly, Category = "Visual")
	class UDecalComponent* TargetDecal;

	float TargettingRange;

	FHitResult GetPlayerView() const;
};
