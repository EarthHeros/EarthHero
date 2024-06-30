

#include "StatCalculationLibrary.h"

#include "StatStructure.h"

void UStatCalculationLibrary::CalNormalDamage(FStatStructure& HeroStat, float inDamage)
{

	//실질 데미지 = 체력 - 데미지 
	float RealDamage = HeroStat.Health - inDamage;
	if (RealDamage > 0)
	{
		HeroStat.Health -= inDamage;
	}
	else
	{
		HeroStat.Health = 0;
	}
}
