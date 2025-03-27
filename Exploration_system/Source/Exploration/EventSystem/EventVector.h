// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "EventVector.generated.h"

//UCLASS(Blueprintable)
//class EXPLORATION_API UEventNode : public UObject
//{
//	GENERATED_BODY()
//
//public:
//	UEventNode();
//
//	void Init(FString EventId, UActivity* Event);
//
//private:
//	FString Id;
//	UActivity* EventPtr;
//};

UCLASS()
class EXPLORATION_API AEventVector : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEventVector();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	TMap<FString, class UActivity*> Events;

public:
	void EventRegister(FString& EventId, class UActivity* Event);

	class UActivity* GetEvent(FString EventId);

	static AEventVector* FindEventManager(UWorld* World);
};
