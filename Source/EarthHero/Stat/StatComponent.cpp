// Fill out your copyright notice in the Description page of Project Settings.


#include "StatComponent.h"

// Sets default values for this component's properties
UStatComponent::UStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	Health = MaxHealth;
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UStatComponent::DamageTaken);
	
}


// Called every frame
void UStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UStatComponent::DamageTaken(AActor* DamagedActor, float InDamage, const UDamageType* DamageType,
	AController* Instigator, AActor* DamageCausor)
{
	// 입력된 데미지가 0이하이면 무시한다.
	if (InDamage <= 0.f)
	{
		return ;
	}

	//데미지 받는 함수 만들어야 한다.
	Health = Health - InDamage;
	UE_LOG(LogTemp, Warning, TEXT("Health : %f"), Health);
	if (Health <= 0.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Health : %f, Character Died."), Health);
	}
}

