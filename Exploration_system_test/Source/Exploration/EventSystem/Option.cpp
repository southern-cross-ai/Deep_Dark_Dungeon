// Fill out your copyright notice in the Description page of Project Settings.
#include "Option.h"
#include "Activity.h"

UOption::UOption()
{
    OptionEvent = nullptr;
    IsEnabled = false;
}

UOption::~UOption()
{
    UE_LOG(LogTemp, Warning, TEXT("UOption is being destroyed"));
}

float UOption::RateCalculator()
{
    return SuccessLevel + FMath::RandRange(0.1f, 0.7f);
}

void UOption::TriggerOutcome()
{
    return;
}

void UOption::CheckAvailability()
{
    if (OptionName.IsEmpty())
    {
        IsEnabled = false;
    }
    else
    {
        IsEnabled = true;
    }
}

void UOption::UpdateSuccessLevel()
{
    SuccessLevel = 1.0f;
}