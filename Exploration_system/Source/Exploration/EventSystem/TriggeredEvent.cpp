// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggeredEvent.h"
#include "Components/ActorComponent.h"

void UTriggeredEvent::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Log, TEXT("Handling event"));
	EventDisplay();
}

void UTriggeredEvent::BeginPlay() {
	Super::BeginPlay();
	if (AActor* Owner = GetOwner()) {
		StaticMeshComponent = Owner->FindComponentByClass<UStaticMeshComponent>();
		if (StaticMeshComponent != nullptr) {
			StaticMeshComponent->SetCollisionProfileName(TEXT("trigger"));
			StaticMeshComponent->SetGenerateOverlapEvents(true);

			StaticMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &UTriggeredEvent::OnOverlapBegin);
		}
	}
	
}