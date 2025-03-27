// Fill out your copyright notice in the Description page of Project Settings.


#include "MyUserWidget.h"

void UMyUserWidget::NativeConstruct()
{
    Super::NativeConstruct();
}

void UMyUserWidget::InitializeComponentWithInput(const FString& InputString)
{
    String1 = InputString;
    UE_LOG(LogTemp, Warning, TEXT("Initialized with: %s"), *String1);
}

void UMyUserWidget::InitializeComponentWithInput2(const FString& InputText)
{
    String2 = InputText;
    UE_LOG(LogTemp, Warning, TEXT("Initialized with: %s"), *String2);
}

void UMyUserWidget::InitializeComponentWithInput3(const TArray<FString>& InputTarray)
{
    Options = InputTarray;
    // UE_LOG(LogTemp, Warning, TEXT("Initialized with: %s"), *Options);
}

void UMyUserWidget::InitializeComponentWithInput4(const TArray<float>& InputTarray)
{
    Probability = InputTarray;
    // UE_LOG(LogTemp, Warning, TEXT("Initialized with: %s"), *Options);
}

void UMyUserWidget::InitializeComponentWithInput5(const FString& InputPortrait)
{
    String3 = InputPortrait;
    UE_LOG(LogTemp, Warning, TEXT("Initialized with: %s"), *String3);
}

