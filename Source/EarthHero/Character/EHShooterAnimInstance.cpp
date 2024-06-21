// Fill out your copyright notice in the Description page of Project Settings.


#include "EHShooterAnimInstance.h"

#include "EHShooter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UEHShooterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Shooter = Cast<AEHShooter>(TryGetPawnOwner());
}

void UEHShooterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if(Shooter == nullptr)
	{
		Shooter = Cast<AEHShooter>(TryGetPawnOwner());
	}
	if(Shooter == nullptr) return;

	FVector Velocity = Shooter->GetVelocity();
	Velocity.Z = 0.f;
	Speed = Velocity.Size();

	bIsInAir = Shooter->GetCharacterMovement()->IsFalling();
	bIsAccelerating = Shooter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f ? true : false;
	bAiming = true;

	// GetBaseAimRotation : 현재 PlayerController가 조준중인 월드 로테이션을 반환한다.
	FRotator AimRotation = Shooter->GetBaseAimRotation();
	FRotator ActorRotation = Shooter->GetActorRotation();
	FRotator VelocityRotation = UKismetMathLibrary::MakeRotFromX(Velocity);

	FRotator DirectionDeltaRotation = UKismetMathLibrary::NormalizedDeltaRotator(ActorRotation, VelocityRotation);
	Direction = DirectionDeltaRotation.Yaw * (-1.f);

	FRotator AimDeltaRotation = UKismetMathLibrary::NormalizedDeltaRotator(AimRotation, ActorRotation);
	YawOffset = AimDeltaRotation.Yaw;
	PitchOffset = AimDeltaRotation.Pitch;
	RollOffset = AimDeltaRotation.Roll;
}
