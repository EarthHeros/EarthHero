// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameHUD.generated.h"

/**
 * 
 */
UCLASS()
class EARTHHERO_API UInGameHUD : public UUserWidget
{
	GENERATED_BODY()

private:
	float GetHealthPercent() const;

public:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar *HealthBar;

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
};
