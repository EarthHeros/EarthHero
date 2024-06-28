// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "StatStructure.generated.h"

USTRUCT(BlueprintType)
struct FStatStructure : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	USkeletalMesh *WeaponMesh = nullptr;	//무기 스켈레탈 매쉬

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MaxHealth = 100.f;				//최대 체력

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float Health = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float HealthRegeneration = 0.f;			//체력 재생

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float AttackPower = 0.f;				//공격력

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float Damage = 0.f;						//데미지

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float AttackSpeed = 0.f;				//공격 속도
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MovementSpeed = 0.f;				//이동 속도

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float DefensePower = 0.f;				//방어력

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MaxEXP = 0.f;						//최대 경험치
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float SkillRange = 0.f;					//스킬 범위
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float ReduceCoolTime = 0.f;				//쿨타임 감소

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	bool HasInvincibilityAbility = 0.f;		//무적 능력

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float Barrier = 0.f;					//베리어
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 Summon = 0;						//소환수
};
