
#pragma once

#include "CoreMinimal.h"
#include "EHCharacterBase.h"
#include "Components/PostProcessComponent.h"
#include "EHCharacter.generated.h"

class UWidgetComponent;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class EARTHHERO_API AEHCharacter : public AEHCharacterBase
{
	GENERATED_BODY()

public:
	AEHCharacter();
	virtual void Tick(float DeltaSeconds) override;;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void EnableForceFieldEffect() const;
	void DisableForceFieldEffect() const;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	UWidgetComponent* OverheadWidget;

	UPROPERTY(EditAnywhere, Category = "ForceField")
	UPostProcessComponent* ForceFieldPostProcessComponent;

	UPROPERTY()
	UMaterialInterface* ForceFieldPostProcessMaterial;
};
