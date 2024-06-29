// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EHCharacter.h"
#include "EHShooter.generated.h"


UCLASS()
class EARTHHERO_API AEHShooter : public AEHCharacter
{
	GENERATED_BODY()

public:
	AEHShooter();
	virtual void Tick(float DeltaSeconds) override;
	virtual void Shoot() override;

protected:
	void DrawShootingLine();

	void ResetFire();
	UFUNCTION(Server, Reliable)
	void Server_Shoot(FVector Start, FVector End);
	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_Shoot(FVector Start, FVector End);
	
private:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float MaxPitchAngle;
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float MinPitchAngle;

	FTimerHandle ShootTimerHandle;
	bool bCanFire = true;
};
