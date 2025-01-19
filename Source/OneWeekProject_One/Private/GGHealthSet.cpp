// Fill out your copyright notice in the Description page of Project Settings.

#include "GGHealthSet.h"
#include "GameplayEffectExtension.h"
#include <Net/UnrealNetwork.h>

UGGHealthSet::UGGHealthSet()
	:
	Health(40.0f),
	MaxHealth(100.0f),
	HealthRegenRate(1.0f),
	Shield(0.0f),
	MaxShield(50.0f),
	ShieldRegenRate(1.0f),
	ShieldBreakCooldown(5.0f),
	ShieldAbsorptionRate(1.0f),
	DamageResistance(0.0f),
	LifeSteal(0.0f),
	CritChance(0.0f),
	CritMultiplier(2.0f),
	CritResistance(0.0f),
	DamageAdd(0.0f),
	DamageMulti(1.0f)
{ }

void UGGHealthSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);
	ClampAttributeOnChange(Attribute, NewValue);
}

void UGGHealthSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	ClampAttributeOnChange(Attribute, NewValue);
}

void UGGHealthSet::ClampAttributeOnChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
	else if (Attribute == GetShieldAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxShield());
	}
}

void UGGHealthSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	UE_LOG(LogTemp, Warning, TEXT("PostGameplayEffectExecute Triggered!"));

	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetInDamageAttribute())
	{
		UE_LOG(LogTemp, Warning, TEXT("PostGameplayEffectExecute Triggered! Damage Received: %f"), GetInDamage());

		float InDamageDone = GetInDamage();

		// is the damage greater then 0?
		if (InDamageDone > 0.0f)
		{
			// checking if there are any listeners for the OnDamageTaken event before broadcasting the damage.
			if (OnDamageTaken.IsBound())
			{
				const FGameplayEffectContextHandle& EffectContext = Data.EffectSpec.GetEffectContext();
				AActor* Instigator = EffectContext.GetOriginalInstigator();
				AActor* Causer = EffectContext.GetEffectCauser();

				OnDamageTaken.Broadcast(Instigator, Causer, Data.EffectSpec.CapturedSourceTags.GetSpecTags(), Data.EvaluatedData.Magnitude);
			}

			// if the shield is greater then 0, then we want to subtract from the shield first then health
			if (GetShield() > 0.0f)
			{
				const float NewSheild = GetShield() - InDamageDone;
				InDamageDone -= GetShield();
				SetShield(FMath::Clamp(NewSheild, 0.0f, GetMaxHealth()));
			}

			/// is the damage still greater then 0 and health is greater then 0?
			if (InDamageDone > 0.0f && GetHealth() > 0.0f)
			{
				const float NewHealth = GetHealth() - InDamageDone;
				SetHealth(FMath::Clamp(NewHealth, 0.0f, GetMaxHealth()));
			}
		}
// 
// 		const FGameplayEffectContextHandle& EffectContext = Data.EffectSpec.GetEffectContext();
// 		const FHitResult* Hit = EffectContext.GetHitResult();
// 		if (Hit)
// 		{
// 			UE_LOG(LogTemp, Warning, TEXT("Hit Bone: %s"), *Hit->BoneName.ToString());
// 		}

		// You're resetting the damage value after applying it to shields and health, so it won't affect future calculations.
		SetInDamage(0.0f);

	}
}

void UGGHealthSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UGGHealthSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGGHealthSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGGHealthSet, HealthRegenRate, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGGHealthSet, Shield, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGGHealthSet, MaxShield, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGGHealthSet, ShieldRegenRate, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGGHealthSet, ShieldBreakCooldown, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGGHealthSet, InDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGGHealthSet, DamageResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGGHealthSet, ShieldAbsorptionRate, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGGHealthSet, LifeSteal, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGGHealthSet, CritMultiplier, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGGHealthSet, CritResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGGHealthSet, DamageAdd, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGGHealthSet, DamageMulti, COND_None, REPNOTIFY_Always);
}

// OnRep_ functions are automatically called when a replicated variable changes on a client.
// This is useful for updating UI or other client-side effects when a variable changes.
void UGGHealthSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGGHealthSet, Health, OldHealth);
}

void UGGHealthSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGGHealthSet, MaxHealth, OldMaxHealth);
}

void UGGHealthSet::OnRep_HealthRegenRate(const FGameplayAttributeData& OldHealthRegenRate)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGGHealthSet, HealthRegenRate, OldHealthRegenRate);
}

void UGGHealthSet::OnRep_Shield(const FGameplayAttributeData& OldShield)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGGHealthSet, Shield, OldShield);
}

void UGGHealthSet::OnRep_MaxShield(const FGameplayAttributeData& OldMaxShield)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGGHealthSet, MaxShield, OldMaxShield);
}

void UGGHealthSet::OnRep_ShieldRegenRate(const FGameplayAttributeData& OldShieldRegenRate)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGGHealthSet, ShieldRegenRate, OldShieldRegenRate);
}

void UGGHealthSet::OnRep_ShieldBreakCooldown(const FGameplayAttributeData& OldShieldBreakCooldown)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGGHealthSet, ShieldBreakCooldown, OldShieldBreakCooldown);
}

void UGGHealthSet::OnRep_InDamage(const FGameplayAttributeData& OldInDamage)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGGHealthSet, InDamage, OldInDamage);
}

void UGGHealthSet::OnRep_DamageResistance(const FGameplayAttributeData& OldDamageResistance)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGGHealthSet, DamageResistance, OldDamageResistance);
}

void UGGHealthSet::OnRep_ShieldAbsorptionRate(const FGameplayAttributeData& OldShieldAbsorptionRate)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGGHealthSet, ShieldAbsorptionRate, OldShieldAbsorptionRate);
}

void UGGHealthSet::OnRep_LifeSteal(const FGameplayAttributeData& OldLifeSteal)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGGHealthSet, LifeSteal, OldLifeSteal);
}

void UGGHealthSet::OnRep_CritMultiplier(const FGameplayAttributeData& OldCritMultiplier)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGGHealthSet, CritMultiplier, OldCritMultiplier);
}

void UGGHealthSet::OnRep_CritResistance(const FGameplayAttributeData& OldCritResistance)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGGHealthSet, CritResistance, OldCritResistance);
}

void UGGHealthSet::OnRep_DamageAdd(const FGameplayAttributeData& OldDamageAdd)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGGHealthSet, DamageAdd, OldDamageAdd);
}

void UGGHealthSet::OnRep_DamageMulti(const FGameplayAttributeData& OldDamageMulti)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGGHealthSet, DamageMulti, OldDamageMulti);
}
