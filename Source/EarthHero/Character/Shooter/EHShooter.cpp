// Fill out your copyright notice in the Description page of Project Settings.


#include "EHShooter.h"

#include "ShooterCombatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AEHShooter::AEHShooter()
{
	MaxPitchAngle = 70.f;
	MinPitchAngle = -60.f;

	CombatComponent = CreateDefaultSubobject<UShooterCombatComponent>(TEXT("Shooter Combat Component"));
	CombatComponent->SetShooter(this);
}

void AEHShooter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// 위아래 시야각 제한
	if(Controller)
	{
		FRotator NewRotator = Controller->GetControlRotation();
		float NewPitch;
		if(NewRotator.Pitch < 180)
		{
			NewPitch = FMath::Clamp(NewRotator.Pitch, MinPitchAngle, MaxPitchAngle);
		}
		else
		{
			NewPitch = FMath::Clamp(NewRotator.Pitch-360.f, MinPitchAngle, MaxPitchAngle);
		}
		NewRotator.Pitch = NewPitch;
		Controller->SetControlRotation(NewRotator);
	}
}

void AEHShooter::Shoot()
{
	Super::Shoot();

	CombatComponent->Fire();
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
