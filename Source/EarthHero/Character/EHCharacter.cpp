

#include "EHCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "Components/PostProcessComponent.h"
#include "EarthHero/Stat/StatComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"

AEHCharacter::AEHCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	// 이동하는 방향으로 몸이 회전할 것인지? 속도는?
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// 일단은 테스트 위해 3인칭으로 설정해두었음
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetMesh());
	CameraBoom->TargetArmLength = 600.f;
	CameraBoom->bUsePawnControlRotation = true;
	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidget->SetupAttachment(RootComponent);

	// Create sub-object for force field effect; the post process while inside force field.
	ForceFieldPostProcessComponent = CreateDefaultSubobject<UPostProcessComponent>(TEXT("ForceFieldPostProcessComponent"));
	ForceFieldPostProcessComponent->bEnabled = false; // Initially disabled
	ForceFieldPostProcessComponent->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UMaterial> PostProcessMaterial(TEXT("/Game/Blueprints/HUD/PP_ForceField_Damage.PP_ForceField_Damage"));
	if (PostProcessMaterial.Succeeded())
	{
		ForceFieldPostProcessMaterial = PostProcessMaterial.Object;
		ForceFieldPostProcessComponent->Settings.AddBlendable(ForceFieldPostProcessMaterial, 1.0f);
	}

	//승언 : StatComponent 붙이기
	StatComponent = CreateDefaultSubobject<UStatComponent>(TEXT("StatComponent"));
}

void AEHCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AEHCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void AEHCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AEHCharacter::EnableForceFieldEffect() const
{
	if (IsLocallyControlled() && ForceFieldPostProcessComponent)
	{
		ForceFieldPostProcessComponent->bEnabled = true;
	}
}

void AEHCharacter::DisableForceFieldEffect() const
{
	if (IsLocallyControlled() && ForceFieldPostProcessComponent)
	{
		ForceFieldPostProcessComponent->bEnabled = false;
	}
}
