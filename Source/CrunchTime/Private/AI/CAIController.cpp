// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/CAIController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BrainComponent.h"

#include "Character/CCharacterBase.h"

#include "GameplayAbilities/CAbilityGenericTags.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Touch.h"


ACAIController::ACAIController()
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("AI Perception Component");

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight Config");

	SightConfig->PeripheralVisionAngleDegrees = 60.f;
	SightConfig->SightRadius = 500.f;
	SightConfig->LoseSightRadius = 600.f;
	SightConfig->SetMaxAge(5.f);
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = false;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = false;

	DamageConfig = CreateDefaultSubobject<UAISenseConfig_Damage>("Damage Config");
	DamageConfig->SetMaxAge(5.f);
	
	TouchConfig = CreateDefaultSubobject<UAISenseConfig_Touch>("Touch Config");
	TouchConfig->SetMaxAge(5.f);

	AIPerceptionComponent->ConfigureSense(*SightConfig);

	AIPerceptionComponent->ConfigureSense(*DamageConfig);
	AIPerceptionComponent->ConfigureSense(*TouchConfig);
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
	AIPerceptionComponent->OnTargetPerceptionForgotten.AddDynamic(this, &ACAIController::TargetForgotten);

	ACCharacterBase* PawnAsCharacter = Cast<ACCharacterBase>(GetPawn());
	if (PawnAsCharacter)
	{
		PawnAsCharacter->OnDeadStatusChanged.AddUObject(this, &ACAIController::PawnDeathStatusChanged);
	}
}

void ACAIController::GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const
{
	if (GetCharacter() && GetCharacter()->GetMesh())
	{
		OutLocation = GetCharacter()->GetMesh()->GetSocketLocation(AIVisionAttachSocketName);
		OutRotation = GetCharacter()->GetMesh()->GetSocketRotation(AIVisionAttachSocketName);
		OutRotation.Pitch = 0;
	}
	else
	{
		Super::GetActorEyesViewPoint(OutLocation, OutRotation);
	}
}

void ACAIController::TargetPerceptionUpdated(AActor* Target, FAIStimulus Stimulus)
{
	if (!GetBlackboardComponent())
		return;

	if (Stimulus.WasSuccessfullySensed())
	{
		if (!GetBlackboardComponent()->GetValueAsObject(TargetBBKeyName))
		{
			GetBlackboardComponent()->SetValueAsObject(TargetBBKeyName, Target);
		}
	}
	else
	{
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target))
		{
			if (TargetASC->HasMatchingGameplayTag(UCAbilityGenericTags::GetDeadTag()))
			{
				//UAIPerceptionComponent::FActorPerceptionContainer::TIterator
				auto Iterator = PerceptionComponent->GetPerceptualDataIterator();
				while (Iterator)
				{
					if (Iterator->Value.Target == Target)
					{
						for (FAIStimulus& Stimuli : Iterator->Value.LastSensedStimuli)
						{
							Stimuli.SetStimulusAge(TNumericLimits<float>::Max());
						}
					}
					++Iterator;
				}
			}
		}
	}
}

void ACAIController::TargetForgotten(AActor* Target)
{
	AActor* CurrentTarget = Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(TargetBBKeyName));
	if (CurrentTarget == Target)
	{
		TArray<AActor*> OtherTargets;
		PerceptionComponent->GetPerceivedHostileActors(OtherTargets);
		if (OtherTargets.Num() != 0)
		{
			GetBlackboardComponent()->SetValueAsObject(TargetBBKeyName, OtherTargets[0]);
		}
		else
		{
			GetBlackboardComponent()->ClearValue(TargetBBKeyName);
		}
	}
}

void ACAIController::PawnDeathStatusChanged(bool bIsDead)
{
	if (bIsDead)
	{
		GetBrainComponent()->StopLogic("Dead");
	}
	else
	{
		GetBrainComponent()->StartLogic();
	}
}
