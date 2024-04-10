// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilities/MMC_Damage.h"
#include "GameplayAbilities/CAttributeSet.h"

UMMC_Damage::UMMC_Damage()
{
	AttackDamageCaptureDef.AttributeToCapture = UCAttributeSet::GetAttackDamageAttribute();
	AttackDamageCaptureDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	AttackDamageCaptureDef.bSnapshot = true;

	ArmorCaptureDef.AttributeToCapture = UCAttributeSet::GetArmorAttribute();
	ArmorCaptureDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	ArmorCaptureDef.bSnapshot = true;

	RelevantAttributesToCapture.Add(AttackDamageCaptureDef);
	RelevantAttributesToCapture.Add(ArmorCaptureDef);
}

float UMMC_Damage::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	FAggregatorEvaluateParameters Params;
	Params.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	Params.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	float Damage = 0;
	GetCapturedAttributeMagnitude(AttackDamageCaptureDef, Spec, Params, Damage);

	float Armor = 0;
	GetCapturedAttributeMagnitude(ArmorCaptureDef, Spec, Params, Armor);
	
	float DamageToDeal = Damage - Armor;
	if (DamageToDeal < 0)
	{
		DamageToDeal = 0;
	}

	return -DamageToDeal;
}
