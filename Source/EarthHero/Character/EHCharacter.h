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

    virtual void Shoot();

protected:
    virtual void BeginPlay() override;

    void Initialize();
    
private:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Camera")
    UCameraComponent* FPSCamera;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Mesh")
    USkeletalMeshComponent* FirstPersonHand;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Weapon")
    USkeletalMeshComponent* WeaponMesh;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
    UWidgetComponent* OverheadWidget;

    UPROPERTY(EditAnywhere, Category = "ForceField")
    UPostProcessComponent* ForceFieldPostProcessComponent;
    
	UPROPERTY(VisibleAnywhere, Category = "Component")
	class UStatComponent* StatComponent;

    UPROPERTY()
    UMaterialInterface* ForceFieldPostProcessMaterial;

    UPROPERTY()
    ABP_BossZone* BossZone;

    UPROPERTY()
    bool bIsInBossZone;

    UPROPERTY()
    bool bIsInForceField;

public:
    FORCEINLINE USkeletalMeshComponent* GetEquippedWeapon() { return WeaponMesh; }
    FORCEINLINE USkeletalMeshComponent* GetFirstPersonMesh() { return FirstPersonHand; }
    FORCEINLINE UCameraComponent* GetFPSCamera() { return FPSCamera; }
};