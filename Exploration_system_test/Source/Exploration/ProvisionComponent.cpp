#include "ProvisionComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/MaterialInterface.h"
#include "Engine/StaticMesh.h"

UProvisionComponent::UProvisionComponent()
{
    PrimaryComponentTick.bCanEverTick = false; // Ticking not needed with setter

    // Set a default value for ProvisionScore
    ProvisionScore = 1000.0f; // You can adjust this default value

    // Initialize the Sphere Component (existing)
    ProvisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ProvisionSphere"));
    ProvisionSphere->SetupAttachment(this);
    ProvisionSphere->SetSphereRadius(ProvisionScore);
    ProvisionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    ProvisionSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

    // Set visibility options for the sphere
    ProvisionSphere->SetHiddenInGame(true); // Visible in game
    ProvisionSphere->SetVisibility(true);    // Visible in editor

    // Optionally set the sphere color for better visibility
    ProvisionSphere->SetRenderCustomDepth(true);
    ProvisionSphere->SetCustomDepthStencilValue(1); // Adjust as needed

    // Initialize the Static Mesh Component
    ProvisionCircleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProvisionCircleMesh"));
    ProvisionCircleMesh->SetupAttachment(this);
    ProvisionCircleMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    ProvisionCircleMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

    // Ensure the mesh is visible in game and editor
    ProvisionCircleMesh->SetHiddenInGame(false);
    ProvisionCircleMesh->SetVisibility(true);
}

void UProvisionComponent::BeginPlay()
{
    Super::BeginPlay();

    // Update the sphere radius to match the ProvisionScore
    if (ProvisionSphere)
    {
        ProvisionSphere->SetSphereRadius(ProvisionScore);
    }
}

void UProvisionComponent::SetProvisionScore(float NewScore)
{
    ProvisionScore = NewScore;

    // Update the sphere radius
    if (ProvisionSphere)
    {
        ProvisionSphere->SetSphereRadius(ProvisionScore);
    }
}

#if WITH_EDITOR
void UProvisionComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    FName PropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;

    if (PropertyName == GET_MEMBER_NAME_CHECKED(UProvisionComponent, ProvisionScore))
    {
        // Update the sphere radius when ProvisionScore changes
        if (ProvisionSphere)
        {
            ProvisionSphere->SetSphereRadius(ProvisionScore);
        }

        // Update the mesh scale when ProvisionScore changes
        if (ProvisionCircleMesh)
        {
            float Diameter = ProvisionScore * 2.0f;
            float FlatScaleZ = 0.01f; // Maintain flatness
            ProvisionCircleMesh->SetWorldScale3D(FVector(Diameter, Diameter, FlatScaleZ));
        }
    }
}
#endif
