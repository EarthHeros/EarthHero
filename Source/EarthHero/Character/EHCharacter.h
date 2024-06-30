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

    /** Minimap spring arm */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class USpringArmComponent* MinimapSpringArm;

    /** Minimap Scene Capture Component */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class USceneCaptureComponent2D* MinimapCaptureComponent;

    /** Minimap render target */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class UTextureRenderTarget2D* MinimapRenderTarget;

    /** Minimap widget class */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UI, meta = (AllowPrivateAccess = "true"))
    TSubclassOf<UUserWidget> MinimapWidgetClass;

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

    //승언: 임시 컴포넌트 추가, 각 히어로 별 컴포넌트로 대체 예정
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
    class UStatComponent* StatComponent;
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

    UPROPERTY()
    UMaterialInterface* ForceFieldPostProcessMaterial;

    UPROPERTY()
    ABP_BossZone* BossZone;

    UPROPERTY()
    bool bIsInBossZone;

    UPROPERTY()
    bool bIsInForceField;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Minimap", meta = (AllowPrivateAccess = "true"))
    class UPaperSpriteComponent* MinimapSprite;

public:
    FORCEINLINE USkeletalMeshComponent* GetEquippedWeapon() { return WeaponMesh; }
    FORCEINLINE USkeletalMeshComponent* GetFirstPersonMesh() { return FirstPersonHand; }
    FORCEINLINE UCameraComponent* GetFPSCamera() { return FPSCamera; }
    
    USpringArmComponent* GetMinimapSpringArm() const { return MinimapSpringArm; }
    USceneCaptureComponent2D* GetMinimapCaptureComponent() const { return MinimapCaptureComponent; }
};
