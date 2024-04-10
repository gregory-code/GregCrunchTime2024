// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilities/ExecCal_Damage.h"
#include "GameplayAbilities/CAttributeSet.h"

struct CaptureDefinations
{
	FGameplayEffectAttributeCaptureDefinition AttackDamageDef;
	FGameplayEffectAttributeCaptureDefinition ArmorDef;

	CaptureDefinations()
	{
		AttackDamageDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
		AttackDamageDef.AttributeToCapture = UCAttributeSet::GetAttackDamageAttribute();
		AttackDamageDef.bSnapshot = true;

		ArmorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
		ArmorDef.AttributeToCapture = UCAttributeSet::GetArmorAttribute();
		ArmorDef.bSnapshot = true;
	}
};

//static means this function is only for this file 
static CaptureDefinations GetCaptureDefs()
{
	//static here means the varible is the one and only.
	static CaptureDefinations CaptureDef{};
	return CaptureDef;
}

UExecCal_Damage::UExecCal_Damage()
{
	RelevantAttributesToCapture.Add(GetCaptureDefs().AttackDamageDef);
	RelevantAttributesToCapture.Add(GetCaptureDefs().ArmorDef);
}

void UExecCal_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	FAggregatorEvaluateParameters Params;
	Params.SourceTags = ExecutionParams.GetOwningSpec().CapturedSourceTags.GetAggregatedTags();
	Params.TargetTags = ExecutionParams.GetOwningSpec().CapturedTargetTags.GetAggregatedTags();
	float Damage = 0;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetCaptureDefs().AttackDamageDef, Params, Damage);
	
	float Armor = 0;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetCaptureDefs().ArmorDef, Params, Armor);

	float DamageToDeal = Damage - Armor;
	if (DamageToDeal < 0)
	{
		DamageToDeal = 0;
	}

	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UCAttributeSet::GetHealthAttribute(), EGameplayModOp::Additive, -DamageToDeal));
}
