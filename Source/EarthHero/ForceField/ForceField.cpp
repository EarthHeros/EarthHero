#include "ForceField.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/StaticMesh.h"
#include "Materials/Material.h"

AForceField::AForceField()
{
	PrimaryActorTick.bCanEverTick = true;

	ForceFieldMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ForceFieldMesh"));
	RootComponent = ForceFieldMesh;

	/*
	// Find and set the static mesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/Path/To/Your/Mesh"));
	if (MeshAsset.Succeeded())
	{
		ForceFieldMesh->SetStaticMesh(MeshAsset.Object);
	}

	// Find and set the material
	static ConstructorHelpers::FObjectFinder<UMaterial> MaterialAsset(TEXT("/Game/Path/To/Your/Material"));
	if (MaterialAsset.Succeeded())
	{
		ForceFieldMesh->SetMaterial(0, MaterialAsset.Object);
	}
	*/

	ExpansionDuration = 2000.0f;  // Example duration, increase to slow down the expansion
	InitialScale = FVector(0.1f, 0.1f, 1.0f);  // Starting scale
	TargetScale = FVector(100.0f, 100.0f, 100.0f);  // Target scale
}

void AForceField::BeginPlay()
{
	Super::BeginPlay();

	if (ExpansionCurve)
	{
		FOnTimelineFloat ProgressFunction;
		ProgressFunction.BindUFunction(this, FName("HandleTimelineProgress"));
		ForceFieldTimeline.AddInterpFloat(ExpansionCurve, ProgressFunction);
		ForceFieldTimeline.SetTimelineLength(ExpansionDuration);
		ForceFieldTimeline.SetLooping(false);
		ForceFieldTimeline.PlayFromStart();
	}
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
