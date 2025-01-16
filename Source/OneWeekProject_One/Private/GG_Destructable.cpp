// Fill out your copyright notice in the Description page of Project Settings.


#include "GG_Destructable.h"
#include "AbilitySystemComponent.h"
#include "GGHealthSet.h"
#include <GameplayEffectTypes.h>

// Sets default values
AGG_Destructable::AGG_Destructable()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetSimulatePhysics(true);
	MeshComp->SetCollisionObjectType(ECC_PhysicsBody);
	RootComponent = MeshComp;

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComp");
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UGGHealthSet>("AttributeSet");
}

// Called when the game starts or when spawned
void AGG_Destructable::BeginPlay()
{
	Super::BeginPlay();

	if (!AbilitySystemComponent) return;

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddUObject(this, &AGG_Destructable::OnHealthAttributeChanged);
	
}

UAbilitySystemComponent* AGG_Destructable::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AGG_Destructable::OnHealthAttributeChanged(const FOnAttributeChangeData& Data)
{
	OnHealthChanged(Data.OldValue, Data.NewValue);
}


