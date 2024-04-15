// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/CMainMenu.h"
#include "Components/Button.h"
#include "Framework/CGameInstance.h"

void UCMainMenu::NativeConstruct()
{
	Super::NativeConstruct();
	Loginbtn->OnClicked.AddDynamic(this, &UCMainMenu::LoginBtnClicked);
	CreateSessionBtn->OnClicked.AddDynamic(this, &UCMainMenu::CreateSessionBtnClicked);
	FindSessionButton->OnClicked.AddDynamic(this, &UCMainMenu::FindSessionBtnClicked);
	CGameInst = GetGameInstance<UCGameInstance>();

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
		CGameInst->CreateSession(FName("MySession"));
	}
}

void UCMainMenu::FindSessionBtnClicked()
{
	if (CGameInst)
	{
		CGameInst->FindSessions();
	}
}
