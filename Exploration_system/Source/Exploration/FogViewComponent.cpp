// Fill out your copyright notice in the Description page of Project Settings.


#include "FogViewComponent.h"
#include "Kismet/KismetMaterialLibrary.h"

// Sets default values for this component's properties
UFogViewComponent::UFogViewComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UFogViewComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UFogViewComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UFogViewComponent::GetHitUVByOwnerLoc(FVector2D& UVResult, float MapSize)
{
	if (!GetOwner()) {
		return false;
	}
	const FVector OwnerLoc = GetOwner()->GetActorLocation();
	float X = FMath::GetMappedRangeValueUnclamped(FVector2D(-MapSize, MapSize), FVector2D(0, 1), OwnerLoc.X);
	float Y = FMath::GetMappedRangeValueUnclamped(FVector2D(-MapSize, MapSize), FVector2D(0, 1), OwnerLoc.Y);
	UVResult.X = X;
	UVResult.Y = Y;
	
	return true;
}

