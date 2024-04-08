// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayEffectTypes.h"
#include "AttributeGuage.generated.h"

/**
 * 
 */
UCLASS()
class UAttributeGuage : public UUserWidget
{
	GENERATED_BODY()
public:
	void UpdateValue(const FOnAttributeChangeData& ChangeData);
	virtual void NativePreConstruct() override;
private:
	UPROPERTY(EditAnywhere, category = "Visual")
	UTexture2D* IconTexture;

	UPROPERTY(meta = (BindWidget))
	class UImage* IconImage;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ValueText;
};
