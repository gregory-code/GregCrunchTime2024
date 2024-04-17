// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SessionEntry.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnsessionBtnClicked, int)
/**
 * 
 */
UCLASS()
class USessionEntry : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	FOnsessionBtnClicked OnSessionBtnClicked;

	void SetSessionInfo(const FString& SessionName, int index);

private:
	int SessionIndex = -1;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SessionNameText;

	UPROPERTY(meta = (BindWidget))
	class UButton* SessionButton;

	UFUNCTION()
	void SessionButtonClicked();
};
