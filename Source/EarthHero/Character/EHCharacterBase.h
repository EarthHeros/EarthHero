
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EHCharacterBase.generated.h"

UCLASS()
class EARTHHERO_API AEHCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AEHCharacterBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
