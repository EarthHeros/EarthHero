// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Stat/StatStructure.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "EHGameInstance.generated.h"

//Need to forward declare classes used 
class FOnlineSessionSearch;
class FOnlineSessionSearchResult;

UCLASS()
class EARTHHERO_API UEHGameInstance : public UGameInstance
{
	GENERATED_BODY()

	virtual void Init() override;

public:
	FStatStructure* GetStatStructure(FName HeroName) const;
	
protected:
	UEHGameInstance();

private:
	class UDataTable* CharacterStatDataTable = nullptr;

public:
	bool IsCheckedPrivate = false;



protected:
	FName JoinedSessionName;
	FString LeaveSessionReason, FindSessionReason;

	void FindSessions(FString Reason);

	// Callback function. This function will run when the session is found.
	void HandleFindSessionsCompleted(bool bWasSuccessful, TSharedRef<FOnlineSessionSearch> Search);

	//Delegate to bind callback event for when sessions are found.
	FDelegateHandle FindSessionsDelegateHandle;

	// This is the connection string for the client to connect to the dedicated server.
	FString ConnectString;

	// This is used to store the session to join information from the search. You could pass it as a paramter to JoinSession() instead. 
	FOnlineSessionSearchResult* SessionToJoin;

	// Function to join the EOS session. 
	void JoinSession();

	// Callback function. This function will run when the session is joined. 
	void HandleJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	// Delegate to bind callback event for join session.
	FDelegateHandle JoinSessionDelegateHandle;

public:
	void LeaveMainSession(FString Reason);

protected:
	void DestroySessionComplete(FName SessionName, bool bWasSuccessful);
	FDelegateHandle DestroySessionCompleteDelegatesHandle;
};
