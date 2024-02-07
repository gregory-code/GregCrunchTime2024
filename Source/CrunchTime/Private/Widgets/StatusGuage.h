// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StatusGuage.generated.h"

/**
 * 
 */
UCLASS()
class UStatusGuage : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetHealth(float NewHealth, float MaxHealth);
	void SetMana(float NewMana, float MaxMana);

private:
	UPROPERTY(meta=(BindWidget))
	class UValueGuage* HealthBar;
	UPROPERTY(meta=(BindWidget))
	class UValueGuage* ManaBar;
};
