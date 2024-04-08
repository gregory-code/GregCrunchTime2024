// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/AttributeGuage.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
void UAttributeGuage::UpdateValue(const FOnAttributeChangeData& ChangeData)
{
	ValueText->SetText(FText::AsNumber((int)ChangeData.NewValue));
}

void UAttributeGuage::NativePreConstruct()
{
	Super::NativePreConstruct();
	if (IconTexture)
	{
		IconImage->SetBrushFromTexture(IconTexture);
	}
}
