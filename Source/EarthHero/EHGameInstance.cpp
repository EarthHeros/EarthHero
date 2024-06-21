

#include "EHGameInstance.h"
#include "Engine/DataTable.h"
#include "UObject/ConstructorHelpers.h"

UEHGameInstance::UEHGameInstance()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_GameTable(TEXT("/Game/Data/Character/DT_CharacterStat.DT_CharacterStat"));
	if (DT_GameTable.Succeeded())
	{
		CharacterStatDataTable = DT_GameTable.Object;
	}
}
