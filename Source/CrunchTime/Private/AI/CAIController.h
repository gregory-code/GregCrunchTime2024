// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CAIController.generated.h"

/**
 * 
 */
UCLASS()
class ACAIController : public AAIController
{
	GENERATED_BODY()
public:
	ACAIController();
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual void BeginPlay() override;
private:
	UPROPERTY(VisibleDefaultsOnly, Category = "AI")
	class UAIPerceptionComponent* AIPerceptionComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = "AI")
	class UAISenseConfig_Sight* SightConfig;

	UPROPERTY(VisibleDefaultsOnly, Category = "AI")
	class UAISenseConfig_Damage* DamageConfig;

	UPROPERTY(VisibleDefaultsOnly, Category = "AI")
	class UAISenseConfig_Touch* TouchConfig;

	UPROPERTY(EditDefaultsOnly, Category  = "AI")
	class UBehaviorTree* BehaviorTree;
	
	UPROPERTY(EditDefaultsOnly, Category  = "AI")
	FName TargetBBKeyName = "Target";

	UFUNCTION()
	void TargetPerceptionUpdated(AActor* Target, FAIStimulus Stimulus);
};
