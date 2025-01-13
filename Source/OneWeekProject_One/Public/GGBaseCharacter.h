// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "GGBaseCharacter.generated.h"

// UCLASS(): A macro that declares a class as an Unreal Engine class that can be used in Blueprints and the editor. 
// This marks the class for automatic Unreal Engine reflection, which is important for things like Blueprint scripting and serialization.
// AGGBaseCharacter: This class inherits from ACharacter, Unreal base class for humanoid characters, and IAbilitySystemInterface, 
// an interface that integrates the class with GAS (Gameplay Ability System). The character will have basic Unreal features like 
// movement, health, and abilities.

class UGGHealthSet;

UCLASS()
class ONEWEEKPROJECT_ONE_API AGGBaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

protected:

	//UAbilitySystemComponent* AbilitySystemComponent: This is a pointer to the UAbilitySystemComponent, which is a key part of GAS. 
	// Its responsible for managing abilities, gameplay effects, and attributes. This component is used to activate abilities, apply effects, 
	// and track the character's status (e.g., health or damage resistance).
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS", meta = (AllowPrivateAccess = "true"))
	class UAbilitySystemComponent* AbilitySystemComponent;	
	
	// This is a pointer to an instance of a custom class (UGGHealthSet). It's a container for the characters health and shield attributes 
	// (and possibly other gameplay attributes). This class handles the logic and changes for things like health or shields during gameplay.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS", meta = (AllowPrivateAccess = "true"))
	UGGHealthSet* HealthSet;

	// TArray<TSubclassOf<class UGGGameplayAbility>> DefaultAbilities: This array holds references to the classes of the abilities that the 
	// character will have by default (like special attacks, healing, or buffs). These abilities are likely subclasses of UGGGameplayAbility,
	// which is a custom ability class.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
	TArray<TSubclassOf<class UGGGameplayAbility>> DefaultAbilities;

	// TArray<TSubclassOf<class UGameplayEffect>> DefaultEffects: Similarly, this array holds references to gameplay effects that will be 
	// applied to the character by default (like damage over time, debuffs, or buffs). 
	// These effects are subclasses of UGameplayEffect.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
	TArray<TSubclassOf<class UGameplayEffect>> DefaultEffects;

public:
	// Sets default values for this character's properties
	AGGBaseCharacter();

protected:
	// This function is called when the character is spawned or the game begins. 
	// Its commonly used to initialize the character, set up any needed states, or apply initial gameplay effects.
	virtual void BeginPlay() override;

	// UAbilitySystemComponent* GetAbilitySystemComponent() const override: This function is part of the IAbilitySystemInterface 
	// (the interface the class implements). It ensures that the class can provide its AbilitySystemComponent to the system. 
	// This is required for GAS to function properly, as it needs to interact with the component to manage abilities and attributes.
	UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// These are custom functions designed to initialize the characters abilities and effects at the start of the game or when the
	// character is first spawned. These methods likely handle adding the default abilities and effects to the AbilitySystemComponent.
	virtual void InitializeAbilities();
	virtual void InitializeEffects();

	// This function is called after all components of the actor are initialized. This can be used for additional setup, especially for setting 
	// up any interdependencies between components.
	virtual void PostInitializeComponents() override;
	
	// This function is triggered when the character's damage attributes change. It receives the damage instigator, the causer,
	// a tag container (which could include tags for different types of damage), and the damage amount.
	virtual void OnDamageTakenChanged(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayTagContainer& GameplayerTagContainer, float Damage);

	// This is a Blueprint implementable event, meaning you can create a Blueprint version of this function to execute visual effects, sounds, 
	// or gameplay reactions when damage is taken. This is where your custom logic for how the character reacts to damage would go in Blueprints.
	UFUNCTION(BlueprintImplementableEvent, Category = "GAS")
	void OnDamageTaken(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayTagContainer& GameplayTagContainer, float Damage);

	// This function is called when the health attribute changes. It likely triggers when health is updated due to abilities, 
	// damage, healing, or effects.
	virtual void OnHealthAttributeChanged(const FOnAttributeChangeData& Data);

	// Another Blueprint implementable event, this one is triggered when health changes. It gives you the old value and the new value of the
	// health attribute, and you can use it in Blueprints to trigger UI updates, visual feedback (like health bars), or specific in-game reactions
	// (like sound effects).
	UFUNCTION(BlueprintImplementableEvent, Category = "GAS")
	void OnHealthChanged(float OldValue, float NewValue);

	// This function is similar to OnHealthAttributeChanged(), but for the shield attribute. It’s triggered when the shield value changes, 
	// which might happen when damage is blocked or a shield is applied.
	virtual void OnShieldAttributeChanged(const FOnAttributeChangeData& Data);

	// This is another Blueprint implementable event, triggered when the shield value changes. It gives you the old and new shield values, 
	// which can be used to trigger feedback or update the UI.
	UFUNCTION(BlueprintImplementableEvent, Category = "GAS")
	void OnShieldChanged(float OldValue, float NewValue);
};
