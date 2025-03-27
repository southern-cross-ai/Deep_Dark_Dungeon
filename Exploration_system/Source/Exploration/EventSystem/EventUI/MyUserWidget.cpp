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

