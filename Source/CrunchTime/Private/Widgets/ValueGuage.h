// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ValueGuage.generated.h"

/**
 * 
 */
UCLASS()
class UValueGuage : public UUserWidget
{
	GENERATED_BODY()
public:
	//this is used to update the setting in the ui designer.
	virtual void NativePreConstruct() override;

	void SetValue(float NewVal, float NewMaxValue);

private:
	UPROPERTY(EditAnywhere, Category = "Visual")
	FLinearColor FillColor;

	UPROPERTY(VisibleDefaultsOnly, meta=(BindWidget))
	class UProgressBar* ProgressBar;

	UPROPERTY(VisibleDefaultsOnly, meta=(BindWidget))
	class UTextBlock* DisplayText;
};
