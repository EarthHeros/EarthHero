

#include "StatCalculationLibrary.h"

#include "StatStructure.h"

void UStatCalculationLibrary::CalNormalDamage(FStatStructure& HeroStat, float inDamage)
{
	// 무적 능력이 있으면 데미지를 입지 않는다.
	if (HeroStat.HasInvincibilityAbility)
	{
		return ;
	}

	//실질 데미지 = 체력 + 베리어 - (데미지 - 방어럭);
	float RealDamage = HeroStat.Health + HeroStat.Barrier - (inDamage - HeroStat.DefensePower);
	if (RealDamage < 0)
	{
		HeroStat.Health -= RealDamage;
	}
}
