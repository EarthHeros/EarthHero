

#include "StatCalculationLibrary.h"

#include "StatStructure.h"

void UStatCalculationLibrary::CalNormalDamage(FStatStructure& HeroStat, float inDamage)
{
	//데미지 0이하 일 시 무시
	if (inDamage <= 0)
		return ;

	// 남는 방어력이 있으면,
	if (HeroStat.DefensePower > 0)
	{
		float leftDamage = HeroStat.DefensePower - inDamage;

		//방어력을 넘는 데미지가 들어올 시
		if (leftDamage > 0)
		{
			HeroStat.DefensePower = 0;
			//실질 데미지 = 체력 - 데미지 
			float RealDamage = HeroStat.Health - leftDamage;
			if (RealDamage > 0)
			{
				HeroStat.Health -= leftDamage;
			}
			else
			{
				HeroStat.Health = 0;
			}
		}
		else
		{
			HeroStat.DefensePower -= inDamage;
		}
	}
	else
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
}
