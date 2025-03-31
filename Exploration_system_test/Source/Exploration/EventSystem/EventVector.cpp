// Fill out your copyright notice in the Description page of Project Settings.


#include "EventVector.h"
#include "EngineUtils.h"
#include "Activity.h"

//UEventNode::UEventNode() {
//}
//
//void UEventNode::Init(FString EventId, UActivity* Event) {
//	Id = EventId;
//	EventPtr = Event;
//}

// Sets default values
AEventVector::AEventVector()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEventVector::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEventVector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

AEventVector* AEventVector::FindEventManager(UWorld* World)
{
	for (TActorIterator<AEventVector> It(World); It; ++It)
	{
		return *It;
	}
	return nullptr;
}

void AEventVector::EventRegister(FString& EventId, UActivity* Event) {
	if (Event != nullptr) {
		Events.Add(EventId, Event);
	}
	else {
		return;
	}
}

UActivity* AEventVector::GetEvent(FString EventId) {
	if (!Events.Contains(EventId)) {
		return nullptr;
	}
	else {
		return Events[EventId];
	}
}

