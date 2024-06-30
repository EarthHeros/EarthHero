#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPP_Pickup.generated.h"

UCLASS()
class EARTHHERO_API ACPP_Pickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPP_Pickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Function to apply impulse to the pickup
	UFUNCTION(BlueprintCallable, Category = "Pickup")
	void ApplyImpulse();
};
