// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/AbilityGuage.h"

#include "Components/Image.h"

#include "GameplayAbilities/GA_AbilityBase.h"


void UAbilityGuage::SetupOwingAbilityCDO(const UGA_AbilityBase* OwningAbilityCDO)
{
	if (OwningAbilityCDO)
	{
		UTexture2D* IconTexture = OwningAbilityCDO->GetIconTexture();
		if (IconTexture)
		{
			IconImage->SetBrushFromTexture(IconTexture);
		}
	}
}
