// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/CMainMenu.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Components/ScrollBox.h"
#include "Widgets/SessionEntry.h"
#include "Framework/CGameInstance.h"

void UCMainMenu::NativeConstruct()
{
	Super::NativeConstruct();
	Loginbtn->OnClicked.AddDynamic(this, &UCMainMenu::LoginBtnClicked);
	CreateSessionBtn->OnClicked.AddDynamic(this, &UCMainMenu::CreateSessionBtnClicked);
	FindSessionButton->OnClicked.AddDynamic(this, &UCMainMenu::FindSessionBtnClicked);
	CreateSessionBtn->SetIsEnabled(false);

	editText->OnTextChanged.AddDynamic(this, &UCMainMenu::SessionNameTextChanged);

	CGameInst = GetGameInstance<UCGameInstance>();
	CGameInst->OnSessionSearchCompleted.AddUObject(this, &UCMainMenu::SessionSearchCompleted);

}

void UCMainMenu::SessionNameTextChanged(const FText& NewText)
{
	CreateSessionBtn->SetIsEnabled(!NewText.IsEmpty());
}

void UCMainMenu::LoginBtnClicked()
{
	if (CGameInst)
	{
		CGameInst->Login();
	}
}

void UCMainMenu::CreateSessionBtnClicked()
{
	if (CGameInst)
	{
		CGameInst->CreateSession(FName{editText->GetText().ToString()});
	}
}

void UCMainMenu::FindSessionBtnClicked()
{
	if (CGameInst)
	{
		CGameInst->FindSessions();
	}
}

void UCMainMenu::SessionSearchCompleted(const TArray<FOnlineSessionSearchResult>& SearchResults)
{
	int index = 0;
	for (const FOnlineSessionSearchResult& SearchResul : SearchResults)
	{
		FString SessionName;
		SearchResul.Session.SessionSettings.Get(CGameInst->GetSessionNameKey(), SessionName);
		USessionEntry* NewSessionEntry = CreateWidget<USessionEntry>(GetOwningPlayer(), SessionEntryClass);

		if (NewSessionEntry)
		{
			NewSessionEntry->SetSessionInfo(SessionName, index);
			NewSessionEntry->OnSessionBtnClicked.AddUObject(this, &UCMainMenu::LoadSessionWithIndex);
		}

		index++;
	}
}

void UCMainMenu::LoadSessionWithIndex(int index)
{
	if (CGameInst)
	{
		CGameInst->JoinSessionWithSearchResultIndex(index);
	}
}
