#include "ForceField.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "EarthHero/Character/EHCharacter.h"

AForceField::AForceField()
{
    PrimaryActorTick.bCanEverTick = true;

    ForceFieldMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ForceFieldMesh"));
    RootComponent = ForceFieldMesh;
    
    /*** Change Here for Time and Scale of Force Field ***/
    ExpansionDuration = 20.0f;  // Set Duration
    InitialScale = FVector(0.1f, 0.1f, 1.0f);  // Starting scale
    TargetScale = FVector(100.0f, 100.0f, 100.0f);  // Target scale

    static ConstructorHelpers::FObjectFinder<UCurveFloat> Curve(TEXT("/Game/Assets/LevelPrototyping/ForceField/FC_ExpansionCurve.FC_ExpansionCurve"));
    if (Curve.Succeeded())
    {
        ExpansionCurve = Curve.Object;  // Assign directly to the class member

        // Check if the curve has any keys
        if (ExpansionCurve && ExpansionCurve->FloatCurve.Keys.Num() > 0)
        {
            ExpansionCurve->FloatCurve.Keys[0].Time = 0.0f; // Set the time of the first key to 0.0
            ExpansionCurve->FloatCurve.Keys[0].Value = 0.0f; // Set the value of the first key to 0.0

            ExpansionCurve->FloatCurve.Keys[1].Time = 20.0f; // Set the time of the second key to 20.0
            ExpansionCurve->FloatCurve.Keys[1].Value = 1.0f; // Set the value of the second key to 1.0
        }
    }
}


void AForceField::BeginPlay()
{
    Super::BeginPlay();

    if (ExpansionCurve)
    {
        FOnTimelineFloat ProgressFunction;
        ProgressFunction.BindUFunction(this, FName("HandleTimelineProgress"));
        ForceFieldTimeline.AddInterpFloat(ExpansionCurve, ProgressFunction);
        ForceFieldTimeline.SetLooping(false);
        ForceFieldTimeline.PlayFromStart();
    }

    OnActorBeginOverlap.AddDynamic(this, &AForceField::OnOverlapBegin);
    OnActorEndOverlap.AddDynamic(this, &AForceField::OnOverlapEnd);
}

void AForceField::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    ForceFieldTimeline.TickTimeline(DeltaTime);
}

void AForceField::HandleTimelineProgress(float Value)
{
    FVector NewScale = FMath::Lerp(InitialScale, TargetScale, Value);
    ForceFieldMesh->SetWorldScale3D(NewScale);
}

void AForceField::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
    if (OtherActor && OtherActor != this)
    {
        if (AEHCharacter* Character = Cast<AEHCharacter>(OtherActor))
        {
            Character->EnableForceFieldEffect();
        }
    }
}

void AForceField::OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
{
    if (OtherActor && OtherActor != this)
    {
        if (AEHCharacter* Character = Cast<AEHCharacter>(OtherActor))
        {
            Character->DisableForceFieldEffect();
        }
    }
}
