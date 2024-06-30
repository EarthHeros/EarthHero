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
	EquippedWeapon = Shooter->GetEquippedWeapon();
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

	if(EquippedWeapon && Shooter->GetMesh() && Shooter->GetFirstPersonMesh())
	{
		FVector OutPosition;
		FRotator OutRotation;
		
		if(!Shooter->IsLocallyControlled()) // TPS
		{
			// RightHand Socket을 기준으로 Bone Space에서 LeftHandTransform의 위치를 구한다
			LeftHandTransform = EquippedWeapon->GetSocketTransform(FName("LeftHandSocket"), RTS_World);
			Shooter->GetMesh()->TransformToBoneSpace(FName("hand_r"), LeftHandTransform.GetLocation(), FRotator::ZeroRotator, OutPosition, OutRotation);
			LeftHandTransform.SetLocation(OutPosition);
			LeftHandTransform.SetRotation(FQuat(OutRotation));
		}
		else // FPS
		{
			LeftHandTransform = EquippedWeapon->GetSocketTransform(FName("FPSLeftHandSocket"), RTS_World);
			Shooter->GetFirstPersonMesh()->TransformToBoneSpace(FName("hand_r"), LeftHandTransform.GetLocation(), FRotator::ZeroRotator, OutPosition, OutRotation);
			LeftHandTransform.SetLocation(OutPosition);
			LeftHandTransform.SetRotation(FQuat(OutRotation));
		}
	}
}
