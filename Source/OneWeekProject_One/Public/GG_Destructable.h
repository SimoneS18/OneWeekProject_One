// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "GG_Destructable.generated.h"

class UGGHealthSet;

UCLASS()
class ONEWEEKPROJECT_ONE_API AGG_Destructable : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, Meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS", Meta = (AllowPrivateAccess = true))
	class UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS", Meta = (AllowPrivateAccess = true))
	UGGHealthSet* AttributeSet;
	
public:	
	// Sets default values for this actor's properties
	AGG_Destructable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void OnHealthAttributeChanged(const FOnAttributeChangeData& Data);

	UFUNCTION(BlueprintImplementableEvent, Category = "GAS")
	void OnHealthChanged(float Oldvalue, float NewValue);

};
