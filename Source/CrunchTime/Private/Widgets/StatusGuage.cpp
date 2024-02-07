// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/StatusGuage.h"
#include "Widgets/ValueGuage.h"

void UStatusGuage::SetHealth(float NewHealth, float MaxHealth)
{
	HealthBar->SetValue(NewHealth, MaxHealth);
}

void UStatusGuage::SetMana(float NewMana, float MaxMana)
{
	ManaBar->SetValue(NewMana, MaxMana);
}
