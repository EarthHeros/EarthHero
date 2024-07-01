// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/EditableTextBox.h>
#include <Components/ScrollBox.h>
#include "LobbyWidget.generated.h"



enum class EClassType : uint8
{
	Warrior,
	Mechanic,
	Shooter,
	Archor,
	NumberOfClass
};

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

	TArray<UButton*> ClassBtns;
	UPROPERTY(meta = (BindWidget))
	UButton* Warrior_Btn;
	UPROPERTY(meta = (BindWidget))
	UButton* Mechanic_Btn;
	UPROPERTY(meta = (BindWidget))
	UButton* Shooter_Btn;
	UPROPERTY(meta = (BindWidget))
	UButton* Archor_Btn;


	UPROPERTY(meta = (BindWidget))
	UScrollBox* Chat_Scr;
	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* Chat_Etb;

	UFUNCTION()
	void ReadyClicked();

	UFUNCTION()
	void WarriorClicked();
	UFUNCTION()
	void MechanicClicked();
	UFUNCTION()
	void ShooterClicked(); 
	UFUNCTION()
	void ArchorClicked();

	int NumberOfClass = 4;
	
	void ChangeSelectedButton(EClassType ClassType);

	UFUNCTION()
	void ChatTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);




	
public:
	void UpdatePlayerNameList(const TArray<FString>& PlayerNameList);
	void UpdateReadyState(const TArray<bool>& PlayerReadyStateArray);
	void AddChatMessage(const FText& Text);
};
