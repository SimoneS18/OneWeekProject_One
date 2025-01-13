#include "GGBaseCharacter.h"
#include "AbilitySystemComponent.h"
#include "GGGameplayAbility.h"
#include "GGHealthSet.h"

// Sets default values
AGGBaseCharacter::AGGBaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComp");

	HealthSet = CreateDefaultSubobject<UGGHealthSet>("HealthSet");
}

// Called when the game starts or when spawned
void AGGBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	HealthSet->OnDamageTaken.AddUObject(this, &AGGBaseCharacter::OnDamageTakenChanged);

	// subscribe to attribute changing
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(HealthSet->GetHealthAttribute()).AddUObject(this, &AGGBaseCharacter::OnHealthAttributeChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(HealthSet->GetShieldAttribute()).AddUObject(this, &AGGBaseCharacter::OnShieldAttributeChanged);
}

UAbilitySystemComponent* AGGBaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AGGBaseCharacter::InitializeAbilities()
{
	// Give Abilities, Server Only
	if (!HasAuthority() || !AbilitySystemComponent)
		return;

	for (TSubclassOf<UGGGameplayAbility>& Ability : DefaultAbilities)
	{
		FGameplayAbilitySpecHandle SpecHandle = AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability, 1, static_cast<int32>(Ability.GetDefaultObject()->GetAbilityInputID()), this));
	}
}

void AGGBaseCharacter::InitializeEffects()
{
	if (!AbilitySystemComponent)
		return;

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	for (TSubclassOf<UGameplayEffect>& Effect : DefaultEffects)
	{
		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(Effect, 1, EffectContext);
		if (SpecHandle.IsValid())
		{
			FActiveGameplayEffectHandle GEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}

void AGGBaseCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (!AbilitySystemComponent)
		return;

	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	InitializeEffects();
	InitializeAbilities();
}

void AGGBaseCharacter::OnDamageTakenChanged(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayTagContainer& GameplayTagContainer, float Damage)
{
	OnDamageTaken(DamageInstigator, DamageCauser, GameplayTagContainer, Damage);
}

void AGGBaseCharacter::OnHealthAttributeChanged(const FOnAttributeChangeData& Data)
{
	OnHealthChanged(Data.OldValue, Data.NewValue);
}

void AGGBaseCharacter::OnShieldAttributeChanged(const FOnAttributeChangeData& Data)
{
	OnShieldChanged(Data.OldValue, Data.NewValue);
}