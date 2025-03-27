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

    // Function to initialize with a string
    UFUNCTION(BlueprintCallable, Category = "Initialization")
    void InitializeComponentWithInput3(const TArray<FString>& InputTarray);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Options")
    TArray<FString> Options;

    // Function to initialize with a string
    UFUNCTION(BlueprintCallable, Category = "Initialization")
    void InitializeComponentWithInput4(const TArray<float>& InputTarray);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Probabilities")
    TArray<float> Probability;

    UFUNCTION(BlueprintCallable, Category = "Initialization")
    void InitializeComponentWithInput5(const FString& InputPortrait);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variable_3")
    FString String3;

protected:
    virtual void NativeConstruct() override;
};