// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/AbilityGuage.h"

#include "AbilitySystemComponent.h"

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
		SubscribeAbilityCommitedDelegate();
	}
	else
	{
		//make it empty
	}
}

void UAbilityGuage::SubscribeAbilityCommitedDelegate()
{
	UAbilitySystemComponent* OwningASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwningPlayerPawn());
	if (OwningASC)
	{
		if (!OwningASC->AbilityCommittedCallbacks.IsBoundToObject(this))
		{
			OwningASC->AbilityCommittedCallbacks.AddUObject(this, &UAbilityGuage::AbilityCommited);
		}
	}
}

void UAbilityGuage::AbilityCommited(UGameplayAbility* Ability)
{
	if (Ability->GetClass() == AbilityCDO->GetClass())
	{
		float CooldownDuration = 0;
		float CooldownTimeRemaining = 0;
		Ability->GetCooldownTimeRemainingAndDuration(Ability->GetCurrentAbilitySpecHandle(), Ability->GetCurrentActorInfo(), CooldownTimeRemaining, CooldownDuration);
		
		UE_LOG(LogTemp, Warning, TEXT("Ability : %s Committed, with cooldown duration: %f, cooldown time remaining: %f"), *Ability->GetName(), CooldownDuration, CooldownTimeRemaining);
	}
}
