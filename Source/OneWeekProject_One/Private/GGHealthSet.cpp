// Fill out your copyright notice in the Description page of Project Settings.

#include "GGHealthSet.h"
#include "GameplayEffectExtension.h"

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
	CritResistance(0.0f)
{

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
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetInDamageAttribute())
	{
		float InDamageDone = GetInDamage();

		UE_LOG(LogTemp, Warning, TEXT("Damage received: %f"), InDamageDone);

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

		// You're resetting the damage value after applying it to shields and health, so it won't affect future calculations.
		SetInDamage(0.0f);

	}
}
