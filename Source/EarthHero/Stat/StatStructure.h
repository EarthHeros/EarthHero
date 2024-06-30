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
	float MaxHealth = 100.f;				//최대 체력

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float Health = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float HealthRegeneration = 0.f;			//체력 재생

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MovementSpeed = 0.f;				//이동속도
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float NormalDamage = 0.f;				//일반 공격 데미지

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float AttackSpeed = 0.f;				//공격 속도

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float SkillDamage = 0.f;				//스킬 데미지
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MaxExp = 0.f;						//최대 경험치

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float Exp = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float SkillCoolTime = 0.f;				//공격 스킬 쿨타임 감소

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float DashCoolTime = 0.f;				//이동 스킬 쿨타임 감소
	
};
