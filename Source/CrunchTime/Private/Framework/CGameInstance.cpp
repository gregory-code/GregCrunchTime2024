// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/CGameInstance.h"
#include "OnlineSubsystem.h"
#include "Online/OnlineSessionNames.h"
#include "OnlineSessionSettings.h"

#include "Kismet/GameplayStatics.h"

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
		SessionSettings.bUsesPresence = true;
		SessionSettings.NumPublicConnections = 10;
		SessionSettings.Set(GetSessionNameKey(), SessionName.ToString(), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

		SessionPtr->CreateSession(0, SessionName, SessionSettings);
	}

}

void UCGameInstance::FindSessions()
{
	OnlineSessionSearch = TSharedPtr<FOnlineSessionSearch>(new FOnlineSessionSearch);

	OnlineSessionSearch->bIsLanQuery = false;
	OnlineSessionSearch->MaxSearchResults = 10;
	OnlineSessionSearch->QuerySettings.Set(SEARCH_LOBBIES, true, EOnlineComparisonOp::Equals);

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
	SessionPtr->OnJoinSessionCompleteDelegates.AddUObject(this, &UCGameInstance::JoinSessionCompleted);
}

void UCGameInstance::CreateSessionCompleted(FName SessionName, bool bWasCreated)
{
	if (bWasCreated)
	{
		UE_LOG(LogTemp, Warning, TEXT("Made Session %s"), *SessionName.ToString());

		LoadMapAndListen(GameLevel);
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

		//OnSessionSearchCompleted.Broadcast(OnlineSessionSearch->SearchResults);

		JoinSessionWithSearchResultIndex(0);
	}
}

void UCGameInstance::JoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type CompletedResult)
{
	if (CompletedResult == EOnJoinSessionCompleteResult::Success)
	{
		FString SeverURL;
		SessionPtr->GetResolvedConnectString(SessionName, SeverURL);
		GetFirstLocalPlayerController()->ClientTravel(SeverURL, TRAVEL_Absolute);
	}
}

void UCGameInstance::JoinSessionWithSearchResultIndex(int searchResultIndex)
{
	if (searchResultIndex < 0 || searchResultIndex >= OnlineSessionSearch->SearchResults.Num())
	{
		return;
	}

	const FOnlineSessionSearchResult& SearchResult = OnlineSessionSearch->SearchResults[searchResultIndex];

	FString SessionName;
	SearchResult.Session.SessionSettings.Get(GetSessionNameKey(), SessionName);
	SessionPtr->JoinSession(0, FName{ SessionName }, SearchResult);
}

void UCGameInstance::LoadMapAndListen(TSoftObjectPtr<UWorld> MapToLoad)
{
	if (!MapToLoad.IsValid())
	{
		MapToLoad.LoadSynchronous();
	}

	if (MapToLoad.IsValid())
	{
		const FName LevelURL = FName(*FPackageName::ObjectPathToPackageName(MapToLoad.ToString()));
		GetWorld()->ServerTravel(LevelURL.ToString() + "?listen");
	}
}
