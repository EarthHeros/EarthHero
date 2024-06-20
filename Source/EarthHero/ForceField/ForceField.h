#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "ForceField.generated.h"

UCLASS()
class EARTHHERO_API AForceField : public AActor
{
	GENERATED_BODY()
    
public:	
	AForceField();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* ForceFieldMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ForceField")
	UCurveFloat* ExpansionCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ForceField")
	float ExpansionDuration;

	FTimeline ForceFieldTimeline;

	UFUNCTION()
	void HandleTimelineProgress(float Value);

	/*
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	*/

private:
	FVector InitialScale;
	FVector TargetScale;
};
