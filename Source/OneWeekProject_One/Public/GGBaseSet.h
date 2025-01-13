//This class (UGGBaseSet) serves as a foundation for managing gameplay attributes in Unreal's GAS framework. 
// It handles attribute changes and enforces logic, such as pre-change checks, clamping values, and providing 
// getters/setters for attributes via the macro ATTRIBUTE_ACCESSORS.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "GGBaseSet.generated.h"

// This macro defines a set of boilerplate functions that allow you to easily create getter, setter, and other utility methods for attributes in your classes.
// Example usage: If you had an attribute Health in a class UGGBaseSet, calling ATTRIBUTE_ACCESSORS(UGGBaseSet, Health) 
// would automatically generate getter, setter, and other relevant functions for the Health attribute.
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

// This class inherits from UAttributeSet, which is a base class for handling gameplay attributes in GAS. 
//Any class that handles attributes like health, damage, or mana would likely inherit from UAttributeSet.
UCLASS()
class ONEWEEKPROJECT_ONE_API UGGBaseSet : public UAttributeSet
{
	GENERATED_BODY()

protected:
	// These are overridden functions from UAttributeSet. They allow you to intervene just before an attribute value changes.

	// Called before an attributes base value (the value before any gameplay effects are applied) changes.
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	//  Called before any attribute changes at all (whether it's the base value or modified by effects).
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	// Its designed to clamp the value of an attribute to a specified range (likely to prevent values from becoming too large or too small).
	virtual void ClampAttributeOnChange(const FGameplayAttribute& Attribute, float& NewValue) const;
	
};
