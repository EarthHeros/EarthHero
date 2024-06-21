// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EARTHHERO_API UStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStatComponent();

	//EH Stats 참조만 가능하다.

	//무기 스켈레탈 매쉬
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USkeletalMesh *WeaponMesh = nullptr;

	//최대 체력
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float MaxHealth = 100.f;

	//현재 체력
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Health = 0.f;

	//체력 재생
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float HealthRegeneration = 0.f;

	//공격력
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float AttackPower = 0.f;

	//데미지
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Damage = 0.f;

	//공격 속도
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float AttackSpeed = 0.f;

	//이동 속도
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float MovementSpeed = 0.f;

	//방어력
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float DefensePower = 0.f;

	//최대 경험치
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float MaxEXP = 0.f;

	//스킬 범위
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float SkillRange = 0.f;

	//쿨타임 감소
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float ReduceCoolTime = 0.f;

	//무적 능력
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool HasInvincibilityAbility = 0.f;

	//베리어
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Barrier = 0.f;

	//소환수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int Summon = 0;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	
	UFUNCTION()
	void DamageTaken(AActor *DamagedActor, float InDamage, const UDamageType *DamageType, AController *Instigator, AActor *DamageCausor);

};
