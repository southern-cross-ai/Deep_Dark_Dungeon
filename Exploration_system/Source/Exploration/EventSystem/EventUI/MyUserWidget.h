// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyUserWidget.generated.h"

/**
 *
 */
UCLASS()
class EXPLORATION_API UMyUserWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    // Function to initialize with a string
    UFUNCTION(BlueprintCallable, Category = "Initialization")
    void InitializeComponentWithInput(const FString& InputString);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variable_1")
    FString String1;

    // Function to initialize with a string
    UFUNCTION(BlueprintCallable, Category = "Initialization")
    void InitializeComponentWithInput2(const FString& InputText);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variable_2")
    FString String2;

protected:
    virtual void NativeConstruct() override;
};