// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/CGameInstance.h"
#include "OnlineSubsystem.h"
#include "Online/OnlineSessionNames.h"
#include "OnlineSessionSettings.h"

void UCGameInstance::Login()
{
	if (IdentityPtr)
	{
		FOnlineAccountCredentials credientails;
		credientails.Type = "accountportal";
		credientails.Id = "";
		credientails.Token = "";
		IdentityPtr->Login(0, credientails);
	}
}

void UCGameInstance::CreateSession(const FName& SessionName)
{
	if (SessionPtr) 
	{
		FOnlineSessionSettings SessionSettings;
		SessionSettings.bIsDedicated = false;
		SessionSettings.bIsLANMatch = false;
		SessionSettings.bAllowInvites = true;
		SessionSettings.bShouldAdvertise = true;
		SessionSettings.bAllowJoinInProgress = true;
		SessionSettings.bAllowJoinViaPresence = true;
		SessionSettings.bUseLobbiesIfAvailable = true;
		SessionSettings.NumPublicConnections = 10;
		SessionSettings.Set(GetSessionNameKey(), SessionName.ToString(), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

		SessionPtr->CreateSession(0, SessionName, SessionSettings);
	}

}

void UCGameInstance::FindSessions()
{
	OnlineSessionSearch = TSharedPtr<FOnlineSessionSearch>(new FOnlineSessionSearch);
	SessionPtr->FindSessions(0, OnlineSessionSearch.ToSharedRef());
}

void UCGameInstance::Init()
{
	Super::Init();

	OnlineSubsystem = IOnlineSubsystem::Get();
	IdentityPtr = OnlineSubsystem->GetIdentityInterface();
	IdentityPtr->OnLoginCompleteDelegates->AddUObject(this, &UCGameInstance::LoginCompleted);

	SessionPtr = OnlineSubsystem->GetSessionInterface();
	SessionPtr->OnCreateSessionCompleteDelegates.AddUObject(this, &UCGameInstance::CreateSessionCompleted);

	SessionPtr->OnFindSessionsCompleteDelegates.AddUObject(this, &UCGameInstance::FindSessionCompleted);
}

void UCGameInstance::CreateSessionCompleted(FName SessionName, bool bWasCreated)
{
	if (bWasCreated)
	{
		UE_LOG(LogTemp, Warning, TEXT("Made Session %s"), *SessionName.ToString());
	}
}

void UCGameInstance::LoginCompleted(int playerNumber, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error)
{
	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("Logged in"));
	}

}

void UCGameInstance::FindSessionCompleted(bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		for (const FOnlineSessionSearchResult& SearchResult : OnlineSessionSearch->SearchResults)
		{
			FString SessionName = "";
			SearchResult.Session.SessionSettings.Get(GetSessionNameKey(), SessionName);
			UE_LOG(LogTemp, Warning, TEXT("Session: %s with id: %s found"), *SessionName, *SearchResult.GetSessionIdStr());
		}
	}
}
