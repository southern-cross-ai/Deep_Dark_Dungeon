// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Option.h"
#include "Activity.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnConditionalEventTriggered);

UENUM(BlueprintType)
enum class EEventState : uint8
{
	NotOccurred UMETA(DisplayName = "This event has never occurred"),
	IsOccurring UMETA(DisplayName = "This event is occurring"),
	HasOccurred UMETA(DisplayName = "This event has occurred")
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class EXPLORATION_API UActivity : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UActivity();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(EditAnywhere, Category = "Effect")
	bool IfTriggerable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	EEventState CurrentEventState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Content")
	FFilePath JsonFilePath;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnConditionalEventTriggered OnConditionalEventTriggered;

protected:
	UPROPERTY(EditAnywhere, Category = "Effect")
	bool IfChoosable;

	//UPROPERTY(EditAnywhere, Category = "Content")
	//TArray<UOption*> OptionList;

	// UPROPERTY(EditAnywhere, Category = "Content")
	// Event* NextEvent;

	UPROPERTY(VisibleAnywhere, Category = "State")
	int EventCount;

	int32 CurrentIndex;  
	TArray<TSharedPtr<FJsonObject>> EventsArray;  

	class UMyUserWidget* Widget;

public:
	UFUNCTION(BlueprintCallable, Category = "Update")
	void EventDisplay();

	UFUNCTION(BlueprintCallable, Category = "Update")
	void UpdateEventCount();

	UFUNCTION()
	void OnScreenClicked();

protected:
	UFUNCTION(BlueprintCallable, Category = "Update")
	void UpdateState();

	UFUNCTION(BlueprintCallable, Category = "Update")
	void AddEventCount();

	UFUNCTION(BlueprintCallable, Category = "Update")
	void MinusEventCount();

	UFUNCTION(BlueprintCallable, Category = "Update")
	void SetEventCount(int number);

	UFUNCTION()
	void HandleNextEvent();

	UFUNCTION(BlueprintCallable, Category = "Check")
	virtual void CheckTrigger();

	// UFUNCTION(BlueprintCallable, Category = "Update")
	// void UpdateEnable();	

	UFUNCTION(BlueprintCallable, Category = "Check")
	bool InternalCheck();

	void BindMouseInput();

	void UnbindMouseInput();

	FString LoadJsonFromFile();
};

