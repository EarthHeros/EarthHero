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
    
    ExpansionDuration = 20.0f;
    InitialScale = FVector(0.1f, 0.1f, 100.0f);

    static ConstructorHelpers::FObjectFinder<UCurveFloat> Curve(TEXT("/Game/Blueprints/ForceField/FC_ExpansionCurve.FC_ExpansionCurve"));
    if (Curve.Succeeded())
    {
        ExpansionCurve = Curve.Object;
    }
}

void AForceField::BeginPlay()
{
    Super::BeginPlay();

    CurrentScale = InitialScale;
    ForceFieldMesh->SetWorldScale3D(CurrentScale);

    OnActorBeginOverlap.AddDynamic(this, &AForceField::OnOverlapBegin);
    OnActorEndOverlap.AddDynamic(this, &AForceField::OnOverlapEnd);
}

void AForceField::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    ExpandForceField(DeltaTime);
}

void AForceField::ExpandForceField(float DeltaTime)
{
    float ScaleIncrement = DeltaTime * (100.0f / ExpansionDuration);
    FVector NewScale = CurrentScale + FVector(ScaleIncrement, ScaleIncrement, 0.0f);

    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(this);
    CurrentScale = NewScale;
    ForceFieldMesh->SetWorldScale3D(CurrentScale);
}

void AForceField::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
    if (OtherActor && OtherActor != this)
    {
        if (AEHCharacter* Character = Cast<AEHCharacter>(OtherActor))
        {
            Character->SetIsInForceField(true);
        }
    }
}

void AForceField::OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
{
    if (OtherActor && OtherActor != this)
    {
        if (AEHCharacter* Character = Cast<AEHCharacter>(OtherActor))
        {
            Character->SetIsInForceField(false);
        }
    }
}
