// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/CAIController.h"

#include "BehaviorTree/BlackboardComponent.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"


ACAIController::ACAIController()
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("AI Perception Component");

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight Config");

	SightConfig->PeripheralVisionAngleDegrees = 60.f;
	SightConfig->SightRadius = 500.f;
	SightConfig->LoseSightRadius = 600.f;

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = false;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = false;

	AIPerceptionComponent->ConfigureSense(*SightConfig);
}

FGenericTeamId ACAIController::GetGenericTeamId() const
{
	return FGenericTeamId(254);
}

void ACAIController::BeginPlay()
{
	Super::BeginPlay();
	if(BehaviorTree)
		RunBehaviorTree(BehaviorTree);

	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ACAIController::TargetPerceptionUpdated);
}

void ACAIController::TargetPerceptionUpdated(AActor* Target, FAIStimulus Stimulus)
{
	if (!GetBlackboardComponent())
		return;

	if (Stimulus.WasSuccessfullySensed())
	{
		GetBlackboardComponent()->SetValueAsObject(TargetBBKeyName, Target);
	}
	else
	{
		GetBlackboardComponent()->ClearValue(TargetBBKeyName);
	}
}
