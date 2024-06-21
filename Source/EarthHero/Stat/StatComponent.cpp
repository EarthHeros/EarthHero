// Fill out your copyright notice in the Description page of Project Settings.


#include "StatComponent.h"

#include "StatCalculationLibrary.h"
#include "EarthHero/EHGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UStatComponent::UStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);  // 컴포넌트가 네트워크에서 복제될 수 있도록 설정
	// ...
}


// Called when the game starts
void UStatComponent::BeginPlay()
{
	Super::BeginPlay();

	//컨트롤러 가져오기
	// PlayerController = Cast<APlayerController>(GetOwner()->GetInstigatorController());
	// if (PlayerController)
	// {
	// 	UE_LOG(LogTemp, Error, TEXT("StatComponent: No PlayerController"));
	// }
	
	//테이블 내용 가져오기

	/* 직업을 가져오는 코드를 작성해야 한다. 지금은 임시로 모든 생성 캐릭터가 Shooter로 설정 */
	FName HeroName = TEXT("Shooter");

	//스텟 초기화(서버->클라이언트)
	InitializeStatData(HeroName);

	if (GetOwner()->HasAuthority())
	{
		//FString Message = FString::Printf(TEXT("Server Attack Power : %f"), HeroStat.AttackPower);
		//GEngine->AddOnScreenDebugMessage(-1, 1233223.f, FColor::Green, Message);
	}
	else
	{
		//FString Message = FString::Printf(TEXT("Client Attack Power : %f"), HeroStat.AttackPower);
		//GEngine->AddOnScreenDebugMessage(-1, 1233223.f, FColor::Green, Message);
	}

	//TakeDamage를 Delecate 설정
	if (GetOwner())
	{
		GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UStatComponent::DamageTaken);
	}
}


// Called every frame
void UStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UStatComponent::DamageTaken_Implementation(AActor* DamagedActor, float InDamage, const UDamageType* DamageType,
                                                AController* Instigator, AActor* DamageCausor)
{
	//데미지 계산
	UStatCalculationLibrary::CalNormalDamage(HeroStat, InDamage);
	
	FString Message = FString::Printf(TEXT("Health : %f"), HeroStat.Health);
	GEngine->AddOnScreenDebugMessage(-1, 1233223.f, FColor::Green, Message);
	if (HeroStat.Health <= 0.f)
	{
		FString Message = FString::Printf(TEXT("Health : %f"), HeroStat.Health);
		GEngine->AddOnScreenDebugMessage(-1, 1233223.f, FColor::Green, Message);
	}
}

bool UStatComponent::DamageTaken_Validate(AActor* DamagedActor, float InDamage, const UDamageType* DamageType,
	AController* Instigator, AActor* DamageCausor)
{
	// 입력된 데미지가 0이하이면 무시한다.
	if (InDamage <= 0.f)
	{
		return false;
	}
	return true;
}

// GameInstance에 저장된 초기 스텟을 불러와 StatComponent의 Base스텟과 스텟을 초기화하는 함수
void UStatComponent::InitializeStatData_Implementation(FName HeroName)
{
	UEHGameInstance* ABGameInstance = Cast<UEHGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (nullptr == ABGameInstance)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1233223.f, FColor::Green, TEXT("No GameInstacnce"));
		return ;
	}

	// 테이블이 존재한다면, 해당 값으로 스텟을 초기화
	if (FStatStructure *TempStat = ABGameInstance->GetStatStructure(HeroName))
	{
		BaseHeroStat = *TempStat;
		HeroStat = BaseHeroStat;
	}
	else
	{
		// 데이터 테이블에 없는 행일 때
		GEngine->AddOnScreenDebugMessage(-1, 1233223.f, FColor::Green, TEXT("Row %s data doesn't exist"));
		UE_LOG(LogClass, Warning, TEXT("Row %s data doesn't exist."), *HeroName.ToString());
	}
}

bool UStatComponent::InitializeStatData_Validate(FName HeroName)
{
	// 여기에 서버에서의 유효성 검사를 수행합니다.
	// 예: 호출한 클라이언트의 권한 확인 등.
	return true; // 유효성 검사가 성공하면 true를 반환합니다.
}

void UStatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// HeroStat 속성을 복제 목록에 추가, 모든 클라이언트에게 복사
	DOREPLIFETIME(UStatComponent, HeroStat);
	DOREPLIFETIME(UStatComponent, BaseHeroStat);
}

