// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GGBaseSet.h"
#include "AbilitySystemComponent.h"
#include "GGHealthSet.generated.h"

/*
DECLARE_MULTICAST_DELEGATE_FourParams: A Macro that defines a delegate with four parameters. Can have multiple listeners (functions) bound to it
FDamageTakenEvent: Name of this delegate - used to define it in the class

	AActor* EffectInstigator ? The actor responsible for applying the effect (e.g., the player who fired the shot).
	AActor* EffectCauser ? The actor that directly caused the damage (e.g., a projectile that hit the target).
	const FGameplayTagContainer& GameplayTagContainer ? A container holding tags that describe the type of effect (e.g., "Fire", "Poison", etc.).
	float Damage ? The amount of damage dealt.
	Vector HitLocation  ? Helps determine where the damage was applied (e.g., headshot vs. body shot).
	FVector HitNormal  ? Can be useful for knockback effects or directional-based reactions.
*/
DECLARE_MULTICAST_DELEGATE_FourParams(
	FDamageTakenEvent, 
	AActor*,																// EffectInstigator
	AActor*,																// EffectCauser
	const FGameplayTagContainer&,				// GameplayTagContainer
	float																	// Damage
	/*,
	Vector // HitLocation,
	FVector //HitNormal
	*/
)

UCLASS()
class ONEWEEKPROJECT_ONE_API UGGHealthSet : public UGGBaseSet
{
	GENERATED_BODY()
	
public: 
	UGGHealthSet();

	// Health

	// character Health
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UGGHealthSet, Health);

	// character Max Health
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UGGHealthSet, MaxHealth);

	// health that regens over time
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData HealthRegenRate;
	ATTRIBUTE_ACCESSORS(UGGHealthSet, HealthRegenRate);

	// Shield

	// absorbed before the health is start to be taken
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Shield;
	ATTRIBUTE_ACCESSORS(UGGHealthSet, Shield);

	// the max shield the character can have
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxShield;
	ATTRIBUTE_ACCESSORS(UGGHealthSet, MaxShield);

	// the sheild regen time
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData ShieldRegenRate;
	ATTRIBUTE_ACCESSORS(UGGHealthSet, ShieldRegenRate);

	// the time it takes before the shield starts to regen
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData ShieldBreakCooldown;
	ATTRIBUTE_ACCESSORS(UGGHealthSet, ShieldBreakCooldown);

	// Shield Absorption Rate (if shields only absorb a % of damage) - E.g. if its 0.7%, 70% is damage to the shield, 30% hits health
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData ShieldAbsorptionRate;
	ATTRIBUTE_ACCESSORS(UGGHealthSet, ShieldAbsorptionRate);

	// Combat
	// 
	// DDamage Received
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData InDamage;
	ATTRIBUTE_ACCESSORS(UGGHealthSet, InDamage);

	// DamageResistance is an attribute that reduces the amount of incoming damage before it is applied to health or shields.
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData DamageResistance;
	ATTRIBUTE_ACCESSORS(UGGHealthSet, DamageResistance);

	// Lifesteal (for healing when dealing damage)
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData LifeSteal;
	ATTRIBUTE_ACCESSORS(UGGHealthSet, LifeSteal);

	// Critical Hit Resistance (reduces critical damage taken) 
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData CritResistance;
	ATTRIBUTE_ACCESSORS(UGGHealthSet, CritResistance);

	mutable FDamageTakenEvent OnDamageTaken;

protected:
	virtual void ClampAttributeOnChange(const FGameplayAttribute& Attribute, float& NewValue) const override;

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
};
