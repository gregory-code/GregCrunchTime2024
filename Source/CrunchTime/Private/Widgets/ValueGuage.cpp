// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/ValueGuage.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UValueGuage::NativePreConstruct()
{
	Super::NativePreConstruct();
	ProgressBar->SetFillColorAndOpacity(FillColor);
}

void UValueGuage::SetValue(float NewVal, float NewMaxValue)
{
	if (NewMaxValue == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Value Guage: %s is geting a max value of 0, cannot calcuate percent for progress bar"), *GetName());
		return;
	}

	float Percent = NewVal / NewMaxValue;
	ProgressBar->SetPercent(Percent);

	FText Text = FText::Format(FText::FromString("{0}/{1}"),  FText::AsNumber((int)NewVal), FText::AsNumber((int)NewMaxValue));
	DisplayText->SetText(Text);
}
