// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidget.generated.h"

/**
 * 
 */
UCLASS()
class EARTHHERO_API ULobbyWidget : public UUserWidget
{
	GENERATED_BODY()
	
	virtual bool Initialize();

private:

	UPROPERTY(meta = (BindWidget))
	class UButton* Ready_Btn;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ReadyButton_Tb;

	TArray<UTextBlock*> PlayerTexts;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Player1_Txt;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Player2_Txt;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Player3_Txt;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Player4_Txt;

	UFUNCTION()
	void ReadyClicked();
	
public:
	void UpdatePlayerNameList(const TArray<FString>& PlayerNameList);
	void UpdateReadyState(const TArray<bool>& PlayerReadyStateArray);
};
