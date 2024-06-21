// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Stat/StatStructure.h"
#include "EHGameInstance.generated.h"

UCLASS()
class EARTHHERO_API UEHGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	FStatStructure* GetStatStructure(FName HeroName) const;
	
protected:
	UEHGameInstance();

private:
	class UDataTable* CharacterStatDataTable = nullptr;
};
