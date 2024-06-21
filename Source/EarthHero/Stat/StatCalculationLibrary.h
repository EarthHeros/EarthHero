#pragma once

#include "StatCalculationLibrary.generated.h"

struct FStatStructure;

UCLASS()
class EARTHHERO_API UStatCalculationLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION()
	static void CalNormalDamage(FStatStructure &HeroStat, float inDamage);
};