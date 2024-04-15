// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "Interfaces/OnlineIdentityInterface.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "CGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class UCGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	void Login();
	void CreateSession(const FName& SessionName);
	void FindSessions();
	const FName& GetSessionNameKey() const { return SessionNameKey; }

protected:
	virtual void Init() override;

private:

	class IOnlineSubsystem* OnlineSubsystem;
	IOnlineIdentityPtr IdentityPtr;
	IOnlineSessionPtr SessionPtr;

	void CreateSessionCompleted(FName SessionName, bool bWasCreated);
	void LoginCompleted(int playerNumber, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error);
	void FindSessionCompleted(bool bWasSuccessful);

	FName SessionNameKey{ "SessionName" };

	TSharedPtr<FOnlineSessionSearch> OnlineSessionSearch;
};
