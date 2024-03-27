// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/AbilityGuage.h"

#include "AbilitySystemComponent.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

#include "GameplayAbilities/GA_AbilityBase.h"
#include "GameplayAbilities/CAbilitySystemBlueprintLibrary.h"
#include "GameplayAbilities/CAttributeSet.h"

void UAbilityGuage::SetupOwingAbilityCDO(const UGA_AbilityBase* OwningAbilityCDO)
{
	AbilityCDO = OwningAbilityCDO;
	if (AbilityCDO)
	{
		UTexture2D* IconTexture = AbilityCDO->GetIconTexture();
		if (IconTexture)
		{
			if (!IconMat)
			{
				IconMat = IconImage->GetDynamicMaterial();
			}
			IconMat->SetTextureParameterValue(IconTextureMaterialParamName, IconTexture);
		}
		
		CooldownDuration = UCAbilitySystemBlueprintLibrary::GetAbilityStaticCooldownDuration(AbilityCDO);
		FNumberFormattingOptions FormattingOptions;
		FormattingOptions.MaximumFractionalDigits = 1;
		CooldownDurationText->SetText(FText::AsNumber(CooldownDuration, &FormattingOptions));

		float ManaCost = UCAbilitySystemBlueprintLibrary::GetAbilityStaticManaCost(AbilityCDO);
		ManaCostText->SetText(FText::AsNumber(ManaCost, &FormattingOptions));
	
		CooldownCounterText->SetVisibility(ESlateVisibility::Hidden);
		SubscribeAbilityDelegates();
	}
	else
	{
		//make it empty
	}
}

void UAbilityGuage::SubscribeAbilityDelegates()
{
	UAbilitySystemComponent* OwningASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwningPlayerPawn());
	if (OwningASC)
	{
		if (!OwningASC->AbilityCommittedCallbacks.IsBoundToObject(this))
		{
			OwningASC->AbilityCommittedCallbacks.AddUObject(this, &UAbilityGuage::AbilityCommited);
			OwningASC->GetGameplayAttributeValueChangeDelegate(UCAttributeSet::GetManaAttribute()).AddUObject(this, &UAbilityGuage::ManaUpdated);
		}
	}
}

void UAbilityGuage::AbilityCommited(UGameplayAbility* Ability)
{
	if (Ability->GetClass() == AbilityCDO->GetClass())
	{
		CooldownDuration = 0;
		CooldownTimeRemaining = 0;
		Ability->GetCooldownTimeRemainingAndDuration(Ability->GetCurrentAbilitySpecHandle(), Ability->GetCurrentActorInfo(), CooldownTimeRemaining, CooldownDuration);
		
		UE_LOG(LogTemp, Warning, TEXT("Ability : %s Committed, with cooldown duration: %f, cooldown time remaining: %f"), *Ability->GetName(), CooldownDuration, CooldownTimeRemaining);
		
		GetWorld()->GetTimerManager().SetTimer(CooldownTickTimerHandle, this, &UAbilityGuage::TickCooldown, CooldownTickInterval, true);
		FTimerHandle CooldownFinishedHandle;
		GetWorld()->GetTimerManager().SetTimer(CooldownFinishedHandle, this, &UAbilityGuage::CooldownFinished, CooldownTimeRemaining, false);
		CooldownCounterText->SetVisibility(ESlateVisibility::Visible);
	}
}

void UAbilityGuage::TickCooldown()
{
	CooldownTimeRemaining -= CooldownTickInterval;
	float Percent = 1 - CooldownTimeRemaining / CooldownDuration;
	IconMat->SetScalarParameterValue(CooldownPercentMaterialParamName, Percent);
	
	FNumberFormattingOptions FormattingOptions;
	FormattingOptions.MaximumFractionalDigits = CooldownTimeRemaining > 1  ?  0 : 1;
	CooldownCounterText->SetText(FText::AsNumber(CooldownTimeRemaining, &FormattingOptions));
}

void UAbilityGuage::CooldownFinished()
{
	GetWorld()->GetTimerManager().ClearTimer(CooldownTickTimerHandle);
	IconMat->SetScalarParameterValue(CooldownPercentMaterialParamName, 1.f);
	CooldownCounterText->SetVisibility(ESlateVisibility::Hidden);
}

void UAbilityGuage::ManaUpdated(const FOnAttributeChangeData& ChangeData)
{
	float NewMana = ChangeData.NewValue;
	float Cost = UCAbilitySystemBlueprintLibrary::GetActiveAbilityManaCost(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwningPlayerPawn()), AbilityCDO);
	if (NewMana < Cost)
	{
		IconMat->SetScalarParameterValue(NotEnoughManaShadeAlphaMaterialParamName, 1);
	}
	else
	{
		IconMat->SetScalarParameterValue(NotEnoughManaShadeAlphaMaterialParamName, 0);
	}
}
