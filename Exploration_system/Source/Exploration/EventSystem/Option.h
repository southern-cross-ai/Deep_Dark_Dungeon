// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"  
#include "Option.generated.h"

/**
 *
 */
class UActivity;

UCLASS(Blueprintable)
class EXPLORATION_API UOption : public UObject
{
	GENERATED_BODY()
public:
	UOption();
	~UOption();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString OptionName;

	UPROPERTY(EditAnywhere)
	float SuccessLevel;

	UPROPERTY(EditAnywhere)
	FString Description;

	UPROPERTY(EditAnywhere)
	bool IsEnabled;

private:

	UPROPERTY(EditAnywhere)
	UActivity* OptionEvent;

public:
	UFUNCTION(Category = "Check")
	float RateCalculator();

	UFUNCTION(Category = "Update")
	void TriggerOutcome();

	UFUNCTION(Category = "Check")
	void CheckAvailability();

	UFUNCTION(Category = "Update")
	void UpdateSuccessLevel();
};
