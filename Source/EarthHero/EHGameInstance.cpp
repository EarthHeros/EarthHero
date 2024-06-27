

#include "EHGameInstance.h"
#include "Engine/DataTable.h"
#include "UObject/ConstructorHelpers.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "Interfaces/OnlineIdentityInterface.h"

UEHGameInstance::UEHGameInstance()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_GameTable(TEXT("/Game/Data/Character/DT_CharacterStat.DT_CharacterStat"));
	if (DT_GameTable.Succeeded())
	{
		CharacterStatDataTable = DT_GameTable.Object;
	}
}

FStatStructure* UEHGameInstance::GetStatStructure(FName HeroName) const
{
	return CharacterStatDataTable->FindRow<FStatStructure>(HeroName, TEXT(""));
}



void UEHGameInstance::Init()
{
    Super::Init();

    LoadSettings();

    //Ŭ���̾�Ʈ�� ���� ã�� ����
    if (!IsRunningDedicatedServer())
    {
        FindSessions("JoinMainSession");
    }
}

void UEHGameInstance::FindSessions(FString Reason)
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (Subsystem)
    {
        IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
        if (Session.IsValid())
        {
            TSharedRef<FOnlineSessionSearch> Search = MakeShared<FOnlineSessionSearch>();

            Search->QuerySettings.SearchParams.Empty();
            Search->MaxSearchResults = 1000;
            Search->bIsLanQuery = false;

            FindSessionReason = Reason;

            FindSessionsDelegateHandle =
                Session->AddOnFindSessionsCompleteDelegate_Handle(FOnFindSessionsCompleteDelegate::CreateUObject(
                    this,
                    &ThisClass::HandleFindSessionsCompleted,
                    Search));

            UE_LOG(LogTemp, Log, TEXT("Finding session."));

            if (!Session->FindSessions(0, Search))
            {
                UE_LOG(LogTemp, Warning, TEXT("Find session failed"));
            }
        }
    }
}

void UEHGameInstance::HandleFindSessionsCompleted(bool bWasSuccessful, TSharedRef<FOnlineSessionSearch> Search)
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (Subsystem)
    {
        IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
        if (Session.IsValid())
        {
            if (bWasSuccessful)
            {
                UE_LOG(LogTemp, Log, TEXT("Found session : %d"), Search->SearchResults.Num());

                for (auto SessionInSearchResult : Search->SearchResults)
                {
                    FString GameName;
                    bool bKeyValueFound1 = SessionInSearchResult.Session.SessionSettings.Get("GameName", GameName);

                    FString PortNumber;
                    bool bKeyValueFound2 = SessionInSearchResult.Session.SessionSettings.Get("PortNumber", PortNumber);

                    // FString LobbyType;
                     //bool bKeyValueFound3 = SessionInSearchResult.Session.SessionSettings.Get("LobbyType", LobbyType);

                    int32 CurrentPlayers = SessionInSearchResult.Session.SessionSettings.NumPublicConnections - SessionInSearchResult.Session.NumOpenPublicConnections;

                    // �˻����ǿ� �����ϰ� ������ ��ȿ���� Ȯ��
                    if (bKeyValueFound1 && bKeyValueFound2)
                    {
                        if (GameName == "EH2" &&
                            ((FindSessionReason == "JoinMainSession" && PortNumber == "7777") ||
                                (FindSessionReason == "JoinLobby" && PortNumber != "7777")) ||
                            (FindSessionReason == "CreateLobby" && PortNumber != "7777" && CurrentPlayers == 0)
                            )
                        {
                            UE_LOG(LogTemp, Log, TEXT("Valid session : %s, %s, %d"), *FindSessionReason, *PortNumber, CurrentPlayers);

                            // ������ �ʿ��� �÷��� ���� ���� ������ ��ȯ
                            if (Session->GetResolvedConnectString(SessionInSearchResult, NAME_GamePort, ConnectString))
                            {
                                SessionToJoin = &SessionInSearchResult;
                                if (SessionToJoin)
                                {
                                    JoinSession();
                                    break;
                                }
                            }
                        }
                    }
                }
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Find Sessions failed."));
            }

            Session->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsDelegateHandle);
            FindSessionsDelegateHandle.Reset();
        }
    }
}

void UEHGameInstance::JoinSession()
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (Subsystem)
    {
        IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
        if (Session.IsValid())
        {
            JoinSessionDelegateHandle =
                Session->AddOnJoinSessionCompleteDelegate_Handle(FOnJoinSessionCompleteDelegate::CreateUObject(
                    this,
                    &ThisClass::HandleJoinSessionCompleted));

            UE_LOG(LogTemp, Log, TEXT("Joining session."));

            //���� ���� �õ�
            if (!Session->JoinSession(0, "MainSession", *SessionToJoin))
            {
                UE_LOG(LogTemp, Warning, TEXT("Join session failed"));
            }
        }
    }
}

void UEHGameInstance::HandleJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (Subsystem)
    {
        IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
        if (Session.IsValid())
        {
            if (Result == EOnJoinSessionCompleteResult::Success)
            {
                UE_LOG(LogTemp, Log, TEXT("Joined session."));
                if (GEngine)
                {
                    //���ǿ��� ip�ּ� ��������
                    if (!Session->GetResolvedConnectString(SessionName, ConnectString))
                    {
                        UE_LOG(LogTemp, Error, TEXT("Could not get connect string."));
                        return;
                    }

                    if (GEngine)
                        GEngine->AddOnScreenDebugMessage(-1, 600.f, FColor::Yellow, FString::Printf(TEXT("Connect String : %s"), *ConnectString));

                    JoinedSessionName = SessionName;

                    //Dedicated ���� ����
                    FURL DedicatedServerURL(nullptr, *ConnectString, TRAVEL_Absolute);
                    FString DedicatedServerJoinError;
                    auto DedicatedServerJoinStatus = GEngine->Browse(GEngine->GetWorldContextFromWorldChecked(GetWorld()), DedicatedServerURL, DedicatedServerJoinError);
                    if (DedicatedServerJoinStatus == EBrowseReturnVal::Failure)
                    {
                        UE_LOG(LogTemp, Error, TEXT("Failed to browse for dedicated server. Error is: %s"), *DedicatedServerJoinError);
                    }
                }
            }
            Session->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionDelegateHandle);
            JoinSessionDelegateHandle.Reset();
        }
    }
}

void UEHGameInstance::LeaveMainSession(FString Reason)
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (Subsystem)
    {
        IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
        if (Session.IsValid())
        {
            DestroySessionCompleteDelegatesHandle =
                Session->OnDestroySessionCompleteDelegates.AddUObject(this, &ThisClass::DestroySessionComplete);

            LeaveSessionReason = Reason;

            UE_LOG(LogTemp, Log, TEXT("Leave main session"));

            if (!Session->DestroySession(JoinedSessionName))
            {
                UE_LOG(LogTemp, Warning, TEXT("Failed to DestroySession : %s"), *JoinedSessionName.ToString());
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Leave session is not vaild"));
        }
    }
}
void UEHGameInstance::DestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (Subsystem)
    {
        IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
        if (Session.IsValid())
        {
            if (bWasSuccessful)
            {
                FindSessions(LeaveSessionReason);
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Failed to destroy session"));
            }

            Session->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegatesHandle);
            DestroySessionCompleteDelegatesHandle.Reset();
        }
    }

}

void UEHGameInstance::SaveSettings()
{
    FString SavePath = FPaths::ProjectSavedDir() + TEXT("Settings.sav");

    FString SaveString;
    SaveString += FString::FromInt(Resolution) + TEXT("\n");
    SaveString += FString::FromInt(ScreenMode) + TEXT("\n");
    SaveString += FString::FromInt(MaxFrame) + TEXT("\n");
    SaveString += bVSyncEnabled ? TEXT("1\n") : TEXT("0\n");
    SaveString += FString::FromInt(OverallQuality) + TEXT("\n");
    SaveString += FString::FromInt(AntiAliasing) + TEXT("\n");
    SaveString += FString::FromInt(PostProcessing) + TEXT("\n");
    SaveString += FString::SanitizeFloat(MasterVolume) + TEXT("\n");
    SaveString += FString::SanitizeFloat(BackgroundVolume) + TEXT("\n");
    SaveString += FString::SanitizeFloat(SFXVolume) + TEXT("\n");
    SaveString += FString::SanitizeFloat(MouseSensitivity) + TEXT("\n");

    FFileHelper::SaveStringToFile(SaveString, *SavePath);
}


void UEHGameInstance::LoadSettings()
{
    FString SavePath = FPaths::ProjectSavedDir() + TEXT("Settings.sav");
    FString LoadString;

    if (FFileHelper::LoadFileToString(LoadString, *SavePath))
    {
        TArray<FString> ParsedStrings;
        LoadString.ParseIntoArray(ParsedStrings, TEXT("\n"), true);

        if (ParsedStrings.Num() >= 11)
        {
            Resolution = FCString::Atoi(*ParsedStrings[0]);
            ScreenMode = FCString::Atoi(*ParsedStrings[1]);
            MaxFrame = FCString::Atoi(*ParsedStrings[2]);
            bVSyncEnabled = FCString::Atoi(*ParsedStrings[3]) == 1;
            OverallQuality = FCString::Atoi(*ParsedStrings[4]);
            AntiAliasing = FCString::Atoi(*ParsedStrings[5]);
            PostProcessing = FCString::Atoi(*ParsedStrings[6]);
            MasterVolume = FCString::Atof(*ParsedStrings[7]);
            BackgroundVolume = FCString::Atof(*ParsedStrings[8]);
            SFXVolume = FCString::Atof(*ParsedStrings[9]);
            MouseSensitivity = FCString::Atof(*ParsedStrings[10]);
        }
    }
}