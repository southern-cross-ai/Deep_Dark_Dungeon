// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Activity.h"
#include "Components/BoxComponent.h"
#include "Option.h"
#include "Components/PrimitiveComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/EngineTypes.h"
#include "TriggeredEvent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class EXPLORATION_API UTriggeredEvent : public UActivity
{
	GENERATED_BODY()
	
public:
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//UBoxComponent* TriggerBox;
	static TArray<UActivity*> CurEvents;

	UFUNCTION(BlueprintCallable, Category = "Static Variables")
	static UActivity* GetCurEvent(int32 index)
	{
		return CurEvents[index];
	}



	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void BeginPlay() override;

	virtual void CheckTrigger() override;

	UPROPERTY(EditAnywhere, Category = "Effect")
	bool isOverlapType;

	UPROPERTY(EditAnywhere, Category = "Effect")
	bool isChildEvent;

	UPROPERTY(EditAnywhere, Category = "Content")
	TArray<UOption*> OptionList;
protected:
	UStaticMeshComponent* StaticMeshComponent;

	virtual void HandleNextEvent() override;
	
};
