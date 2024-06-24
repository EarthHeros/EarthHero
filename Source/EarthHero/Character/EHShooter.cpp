// Fill out your copyright notice in the Description page of Project Settings.


#include "EHShooter.h"

#include "Camera/CameraComponent.h"
#include "EarthHero/Player/EHPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"

AEHShooter::AEHShooter()
{
	MaxPitchAngle = 70.f;
	MinPitchAngle = -60.f;
}

void AEHShooter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	DrawShootingLine();

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
		UE_LOG(LogTemp, Warning, TEXT("%f"), NewRotator.Pitch);
		NewRotator.Pitch = NewPitch;
		Controller->SetControlRotation(NewRotator);
	}
}

void AEHShooter::DrawShootingLine()
{
	FVector SocketLocation = GetMesh()->GetSocketLocation(FName("FPSCamSocket"));
	FRotator ControlRotation;
	if(Controller)
	{
		ControlRotation = Controller->GetControlRotation();
	}
	FVector Start = SocketLocation;
	FVector End = SocketLocation + (ControlRotation.Vector() * 1000.f);
	
	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, -1, 0, 1.0f);
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
