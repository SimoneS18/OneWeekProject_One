// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GGGameplayEffectDamageCalcu.generated.h"

/**
 * 
 */
UCLASS()
class ONEWEEKPROJECT_ONE_API UGGGameplayEffectDamageCalcu : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
public:

	UGGGameplayEffectDamageCalcu();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
