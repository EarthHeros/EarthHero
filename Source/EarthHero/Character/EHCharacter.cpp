#include "EHCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/PostProcessComponent.h"
#include "EarthHero/Stat/StatComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "../ForceField/BossZone.h"

AEHCharacter::AEHCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    FPSCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FPS Camera"));
    FPSCamera->SetupAttachment(GetMesh(), FName("FPSCamSocket"));
    FPSCamera->bUsePawnControlRotation = true;
    FPSCamera->bUsePawnControlRotation = true;

    OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
    OverheadWidget->SetupAttachment(RootComponent);

    ForceFieldPostProcessComponent = CreateDefaultSubobject<UPostProcessComponent>(TEXT("ForceFieldPostProcessComponent"));
    ForceFieldPostProcessComponent->bEnabled = false;
    ForceFieldPostProcessComponent->SetupAttachment(RootComponent);

    WeaponMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
    WeaponMeshComponent->SetupAttachment(GetMesh(), FName("RightWeaponSocket"));

	//승언 : StatComponent 붙이기
	StatComponent = CreateDefaultSubobject<UStatComponent>(TEXT("StatComponent"));

    static ConstructorHelpers::FObjectFinder<UMaterial> PostProcessMaterial(TEXT("/Game/Blueprints/HUD/PP_ForceField_Damage.PP_ForceField_Damage"));
    if (PostProcessMaterial.Succeeded())
    {
        ForceFieldPostProcessMaterial = PostProcessMaterial.Object;
        ForceFieldPostProcessComponent->Settings.AddBlendable(ForceFieldPostProcessMaterial, 1.0f);
    }

    BossZone = nullptr;
    bIsInBossZone = false;
    bIsInForceField = false;

    GetCharacterMovement()->MaxWalkSpeed = 500.f;
}

void AEHCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    // if(FPSCamera)
    // {
    //     FRotator NewCameraRotation = FRotator(0.f, 90.f, 0.f);
    //     FPSCamera->SetWorldRotation(NewCameraRotation);
    // }
}

void AEHCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void AEHCharacter::BeginPlay()
{
    Super::BeginPlay();

    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABP_BossZone::StaticClass(), FoundActors);
    if (FoundActors.Num() > 0)
    {
        BossZone = Cast<ABP_BossZone>(FoundActors[0]);
    }
}

void AEHCharacter::EnableForceFieldEffect()
{
    if (IsLocallyControlled() && ForceFieldPostProcessComponent && !bIsInBossZone)
    {
        ForceFieldPostProcessComponent->bEnabled = true;
    }
}

void AEHCharacter::DisableForceFieldEffect()
{
    if (IsLocallyControlled() && ForceFieldPostProcessComponent)
    {
        ForceFieldPostProcessComponent->bEnabled = false;
    }
}

bool AEHCharacter::IsInBossZone() const
{
    return bIsInBossZone;
}

void AEHCharacter::SetIsInBossZone(bool bInBossZone)
{
    bIsInBossZone = bInBossZone;
    if (bIsInBossZone)
    {
        DisableForceFieldEffect();
    }
    else if (bIsInForceField)
    {
        EnableForceFieldEffect();
    }
}

bool AEHCharacter::IsInForceField() const
{
    return bIsInForceField;
}

void AEHCharacter::SetIsInForceField(bool bInForceField)
{
    bIsInForceField = bInForceField;
    if (bIsInForceField && !bIsInBossZone)
    {
        EnableForceFieldEffect();
    }
    else
    {
        DisableForceFieldEffect();
    }
}
