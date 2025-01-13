#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "OneWeekProject_One/OneWeekProject_One.h"
#include "GGGameplayAbility.generated.h"

UCLASS()
class ONEWEEKPROJECT_ONE_API UGGGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Ability")
	EAbilityInputID AbilityInputID{ EAbilityInputID::None };

public: 
	EAbilityInputID GetAbilityInputID() { return AbilityInputID;  }
	
};
