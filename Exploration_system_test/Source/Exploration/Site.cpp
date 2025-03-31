#include "Site.h"
#include "Components/StaticMeshComponent.h"

ASite::ASite()
{
    PrimaryActorTick.bCanEverTick = false;

    // Initialize the Static Mesh Component
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;
}

void ASite::BeginPlay()
{
    Super::BeginPlay();
}

void ASite::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
