// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/AbilityGuage.h"

#include "Components/Image.h"

#include "GameplayAbilities/GA_AbilityBase.h"


void UAbilityGuage::SetupOwingAbilitySpec(const FGameplayAbilitySpec* OwningSpec)
{
	const UGA_AbilityBase* OwningAbilityCDO = Cast<UGA_AbilityBase>(OwningSpec->Ability);
	if (OwningAbilityCDO)
	{
		UTexture2D* IconTexture = OwningAbilityCDO->GetIconTexture();
		if (IconTexture)
		{
			IconImage->SetBrushFromTexture(IconTexture);
		}
	}
}
