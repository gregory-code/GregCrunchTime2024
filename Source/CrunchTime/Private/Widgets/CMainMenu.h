// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CMainMenu.generated.h"

/**
 * 
 */
UCLASS()
class UCMainMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* Loginbtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* CreateSessionBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* FindSessionButton;

	UFUNCTION()
	void LoginBtnClicked();

	UFUNCTION()
	void CreateSessionBtnClicked();

	UFUNCTION()
	void FindSessionBtnClicked();

	class UCGameInstance* CGameInst;
};
