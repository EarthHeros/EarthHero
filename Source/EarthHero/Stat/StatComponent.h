#pragma once

#include "EarthHero/EHGameInstance.h"
#include "StatComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EARTHHERO_API UStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//스텟 반환 함수
	UFUNCTION(BlueprintPure, Category = "Stat")
	float GetHealth() const;

	UFUNCTION(BlueprintPure, Category = "Stat")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintPure, Category = "Stat")
	float GetHealthPercent() const;

private:

	//APlayerController *PlayerController;
	//참조만 가능하다.
	//replicated replicateUsing에 대해 좀 더 생각해 봐야한다
	UPROPERTY(Replicated)
	FStatStructure HeroStat;
	
	UPROPERTY(Replicated)
	FStatStructure BaseHeroStat;

	FTimerHandle TimerHandle;
	float TestPoison = 2.0f;

	//데미지 처리 함수
	UFUNCTION(Server, Reliable, WithValidation)
	void DamageTaken(AActor *DamagedActor, float InDamage, const UDamageType *DamageType, AController *Instigator, AActor *DamageCausor);
	
	//초기화 작업을 위한 DataTable과 함수
	UFUNCTION(Server, Reliable, WithValidation)
	void InitializeStatData(FName HeroName);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
