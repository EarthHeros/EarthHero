// Fill out your copyright notice in the Description page of Project Settings.


#include "EHShooter.h"

#include "GameFramework/CharacterMovementComponent.h"

AEHShooter::AEHShooter()
{
}

void AEHShooter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AEHShooter::BeginPlay()
{
	Super::BeginPlay();
}

void AEHShooter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	GetCharacterMovement()->bOrientRotationToMovement = false;
	bUseControllerRotationYaw = true;
}
