// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSessionSettings.h"
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
	class UScrollBox* SessionEntryList;

	UPROPERTY(meta = (BindWidget))
	class UEditableText* editText;

	UPROPERTY(meta = (BindWidget))
	class UButton* CreateSessionBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* FindSessionButton;

	UPROPERTY(EditDefaultsOnly, Category = "Session")
	TSubclassOf<class USessionEntry> SessionEntryClass;

	UFUNCTION()
	void SessionNameTextChanged(const FText& NewText);


	UFUNCTION()
	void LoginBtnClicked();

	UFUNCTION()
	void CreateSessionBtnClicked();

	UFUNCTION()
	void FindSessionBtnClicked();

	class UCGameInstance* CGameInst;
	
	void SessionSearchCompleted(const TArray<FOnlineSessionSearchResult>& SearchResults);
	void LoadSessionWithIndex(int index);
};
