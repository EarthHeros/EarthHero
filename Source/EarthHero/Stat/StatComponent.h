// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EarthHero/EHGameInstance.h"
#include "StatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EARTHHERO_API UStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStatComponent();

	//참조만 가능하다.
	//replicated replicateUsing에 대해 좀 더 생각해 봐야한다
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	FStatStructure HeroStat;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	FStatStructure BaseHeroStat;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	//APlayerController *PlayerController;

	//데미지 처리 함수
	UFUNCTION(Server, Reliable, WithValidation)
	void DamageTaken(AActor *DamagedActor, float InDamage, const UDamageType *DamageType, AController *Instigator, AActor *DamageCausor);
	
	//초기화 작업을 위한 DataTable과 함수
	UFUNCTION(Server, Reliable, WithValidation)
	void InitializeStatData(FName HeroName);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
