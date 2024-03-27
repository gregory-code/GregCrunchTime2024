// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/AbilityGuage.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

#include "GameplayAbilities/GA_AbilityBase.h"
#include "GameplayAbilities/CAbilitySystemBlueprintLibrary.h"


void UAbilityGuage::SetupOwingAbilityCDO(const UGA_AbilityBase* OwningAbilityCDO)
{
	AbilityCDO = OwningAbilityCDO;
	if (AbilityCDO)
	{
		UTexture2D* IconTexture = AbilityCDO->GetIconTexture();
		if (IconTexture)
		{
			IconImage->SetBrushFromTexture(IconTexture);
		}
		float CooldownDuration = UCAbilitySystemBlueprintLibrary::GetAbilityStaticCooldownDuration(AbilityCDO);
		FNumberFormattingOptions FormattingOptions;
		FormattingOptions.MaximumFractionalDigits = 1;
		CooldownDurationText->SetText(FText::AsNumber(CooldownDuration, &FormattingOptions));

		float ManaCost = UCAbilitySystemBlueprintLibrary::GetAbilityStaticManaCost(AbilityCDO);
		ManaCostText->SetText(FText::AsNumber(ManaCost, &FormattingOptions));
	
		CooldownCounterText->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		//make it empty
	}
}
