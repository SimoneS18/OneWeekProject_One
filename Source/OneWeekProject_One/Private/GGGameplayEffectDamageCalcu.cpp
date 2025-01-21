// Fill out your copyright notice in the Description page of Project Settings.

#include "GGGameplayEffectDamageCalcu.h"
#include "GameplayEffectExecutionCalculation.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "GGHealthSet.h"

struct FDamageStatic
{
    DECLARE_ATTRIBUTE_CAPTUREDEF(Health);
	DECLARE_ATTRIBUTE_CAPTUREDEF(InDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CritChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CritMultiplier);
	FDamageStatic()
	{
		// Source: coming from the person doing the damage
		// Target: the person taking the damage (damage resistance)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGGHealthSet, Health, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGGHealthSet, InDamage, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGGHealthSet, CritChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGGHealthSet, CritMultiplier, Source, false);
	}
};

static const FDamageStatic& DamageStatics()
{
	static FDamageStatic DmgStatics;
	return DmgStatics;
}


UGGGameplayEffectDamageCalcu::UGGGameplayEffectDamageCalcu()
{
	RelevantAttributesToCapture.Add(DamageStatics().HealthDef);
	RelevantAttributesToCapture.Add(DamageStatics().InDamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().CritChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CritMultiplierDef);
}


void UGGGameplayEffectDamageCalcu::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UE_LOG(LogTemp, Warning, TEXT("Executing Damage Calculation"));

	UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	AActor* TargetActor = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float TargetHealth = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().HealthDef, EvaluationParameters, TargetHealth);

	//float InDamage = FMath::Max(Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Damage.SetByCaller")), false, -1.0f), 0.0f);

	float InDamage = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().InDamageDef, EvaluationParameters, InDamage);

	FGameplayEffectSpec* MutableSpec = ExecutionParams.GetOwningSpecForPreExecuteMod();;
	float CritChance = 0.0f;
	float CritMultiplier = 0.0f;

	const FHitResult* Hit = Spec.GetEffectContext().GetHitResult();

	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CritChanceDef, EvaluationParameters, CritChance);
	if (Hit && Hit->BoneName == "head")
		CritChance += 100.0f;

	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CritMultiplierDef, EvaluationParameters, CritMultiplier);
	if (CritChance > 1.0f)
	{
		InDamage *= CritMultiplier;
	}

	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().InDamageProperty, EGameplayModOp::Additive, InDamage));

	// Debug message
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Final Damage Applied: %f"), InDamage));
}