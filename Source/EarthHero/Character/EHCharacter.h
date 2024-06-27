#pragma once

#include "CoreMinimal.h"
#include "EHCharacterBase.h"
#include "Components/PostProcessComponent.h"
#include "EHCharacter.generated.h"

class UWidgetComponent;
class UCameraComponent;
class USpringArmComponent;
class ABP_BossZone;

UCLASS()
class EARTHHERO_API AEHCharacter : public AEHCharacterBase
{
    GENERATED_BODY()

    friend class AEHPlayerController;
public:
    AEHCharacter();
    virtual void Tick(float DeltaSeconds) override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    void EnableForceFieldEffect();
    void DisableForceFieldEffect();

    bool IsInBossZone() const;
    void SetIsInBossZone(bool bInBossZone);

    bool IsInForceField() const;
    void SetIsInForceField(bool bInForceField);

protected:
    virtual void BeginPlay() override;

    virtual void PostInitializeComponents() override;
    
    UPROPERTY(VisibleAnywhere, Category = "Camera")
    UCameraComponent* FPSCamera;

    UPROPERTY(VisibleAnywhere,Category = "Mesh")
    USkeletalMeshComponent* FirstPersonHand;
    
    UPROPERTY(VisibleAnywhere, Category = "Weapon")
    USkeletalMeshComponent* WeaponMesh;

private:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
    UWidgetComponent* OverheadWidget;

    UPROPERTY(EditAnywhere, Category = "ForceField")
    UPostProcessComponent* ForceFieldPostProcessComponent;
    
	//승언 : StatComponent 붙이기
	UPROPERTY(VisibleAnywhere, Category="Component")
	class UStatComponent *StatComponent;
    UPROPERTY()
    UMaterialInterface* ForceFieldPostProcessMaterial;

    UPROPERTY()
    ABP_BossZone* BossZone;

    UPROPERTY()
    bool bIsInBossZone;

    UPROPERTY()
    bool bIsInForceField;

    bool bIsFirstPersonCam;
};
