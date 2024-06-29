// Fill out your copyright notice in the Description page of Project Settings.


#include "EHShooter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AEHShooter::AEHShooter()
{
	MaxPitchAngle = 70.f;
	MinPitchAngle = -60.f;
}

void AEHShooter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//DrawShootingLine();

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

	if(!GetEquippedWeapon()) return;

	if(bCanFire)
	{
		bCanFire = false;
	
		FRotator CameraRotation = GetController()->GetControlRotation();
		FVector StartLocation = GetFPSCamera()->GetComponentLocation();
		FVector EndLocation = StartLocation + (CameraRotation.Vector() * 15000.f);
	
		FHitResult HitResult;
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(this);

		bool bHit = GetWorld()->LineTraceSingleByChannel(
			HitResult,
			StartLocation,
			EndLocation,
			ECC_Visibility,
			CollisionParams);
	
		if(bHit)
		{
			FVector Start = GetEquippedWeapon()->GetSocketLocation(FName("FireSocket"));
			FVector End = HitResult.Location;
			Server_Shoot(Start, End);
		}

		GetWorldTimerManager().SetTimer(ShootTimerHandle, this, &ThisClass::ResetFire, 0.1f, false);
	}
}

void AEHShooter::ResetFire()
{
	bCanFire = true;
}

void AEHShooter::Server_Shoot_Implementation(FVector Start, FVector End)
{
	NetMulticast_Shoot(Start, End);	
}

void AEHShooter::NetMulticast_Shoot_Implementation(FVector Start, FVector End)
{
	DrawDebugLine(GetWorld(), Start, End, FColor::Emerald, false, 10.f, 0,1.f);
	DrawDebugPoint(GetWorld(), End, 20.f, FColor::Red, false, 10.f);
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
