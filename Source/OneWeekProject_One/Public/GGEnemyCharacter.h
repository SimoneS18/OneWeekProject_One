// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GGBaseCharacter.h"
#include "GGEnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class ONEWEEKPROJECT_ONE_API AGGEnemyCharacter : public AGGBaseCharacter
{
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	AGGEnemyCharacter();

protected:
	//Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
