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
	class UEditableTextBox* ReadyButton_Tb;

	/*
	UPROPERTY(meta = (BindWidget))
	class UText* Player1_Txt;
	UPROPERTY(meta = (BindWidget))
	UText* Player2_Txt;
	UPROPERTY(meta = (BindWidget))
	UText* Player3_Txt;
	UPROPERTY(meta = (BindWidget))
	UText* Player4_Txt;*/

	UFUNCTION()
	void ReadyClicked();
};
