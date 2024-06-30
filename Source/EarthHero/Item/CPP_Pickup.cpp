// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Pickup.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GenericPlatform/GenericPlatformMath.h"

// Sets default values
ACPP_Pickup::ACPP_Pickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACPP_Pickup::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACPP_Pickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Function to apply impulse to the pickup with random values in the range [200, 400]
void ACPP_Pickup::ApplyImpulse()
{
    UPrimitiveComponent* PickupRoot = Cast<UPrimitiveComponent>(RootComponent);
    if (PickupRoot)

    {
        // Generate random values for x, y, and z in the range [200, 400]
        float RandomX = FMath::RandRange(-400.0f, 400.0f);
        float RandomY = FMath::RandRange(-400.0f, 400.0f);
        float RandomZ = FMath::RandRange(200.0f, 600.0f);

        // Create a FVector with the random values
        FVector ImpulseDirection = FVector(RandomX, RandomY, RandomZ);

        // Add impulse to the pickup
        PickupRoot->AddImpulse(ImpulseDirection, NAME_None, true);
        UKismetSystemLibrary::PrintString(this, "Works");
    }
}
