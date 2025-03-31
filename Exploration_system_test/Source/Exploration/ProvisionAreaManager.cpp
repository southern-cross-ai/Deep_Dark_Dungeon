#include "ProvisionAreaManager.h"
#include "Kismet/GameplayStatics.h"
#include "Site.h"

UProvisionAreaManager::UProvisionAreaManager(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
   
}

void UProvisionAreaManager::Initialize()
{
     World = GetOuter()->GetWorld();
    if (!World)
    {
        UE_LOG(LogTemp, Error, TEXT("World is null in UProvisionAreaManager::Initialize()"));
        return;
    }
    // Get all ASite actors in the world
    TArray<AActor*> SiteActors;
    UGameplayStatics::GetAllActorsOfClass(World, ASite::StaticClass(), SiteActors);

    // Collect all provision components and initialize the parent map
    for (AActor* Actor : SiteActors)
    {
        ASite* Site = Cast<ASite>(Actor);
        if (Site)
        {
            UProvisionComponent* ProvisionComp = Site->FindComponentByClass<UProvisionComponent>();
            if (ProvisionComp)
            {
                ProvisionComponents.Add(ProvisionComp);
                ParentMap.Add(ProvisionComp, ProvisionComp); // Initially, each component is its own parent
            }
        }
    }

    // Now, union overlapping provision components
    for (int32 i = 0; i < ProvisionComponents.Num(); ++i)
    {
        UProvisionComponent* CompA = ProvisionComponents[i];
        FVector LocationA = CompA->GetComponentLocation();
        float RadiusA = CompA->ProvisionScore;

        for (int32 j = i + 1; j < ProvisionComponents.Num(); ++j)
        {
            UProvisionComponent* CompB = ProvisionComponents[j];
            FVector LocationB = CompB->GetComponentLocation();
            float RadiusB = CompB->ProvisionScore;

            // Check if the spheres overlap
            float Distance = FVector::Dist(LocationA, LocationB);
            if (Distance <= (RadiusA + RadiusB))
            {
                // Union the two components
                UnionComponents(CompA, CompB);
            }
        }
    }

    // Assign cluster IDs
    int32 ClusterID = 0;
    for (UProvisionComponent* Comp : ProvisionComponents)
    {
        UProvisionComponent* Root = FindParent(Comp);
        if (!ComponentClusterMap.Contains(Root))
        {
            ComponentClusterMap.Add(Root, ClusterID++);
        }
        ComponentClusterMap.Add(Comp, ComponentClusterMap[Root]);
    }
}

UProvisionComponent* UProvisionAreaManager::FindParent(UProvisionComponent* Component)
{
    if (ParentMap[Component] != Component)
    {
        ParentMap[Component] = FindParent(ParentMap[Component]); // Path compression
    }
    return ParentMap[Component];
}

void UProvisionAreaManager::UnionComponents(UProvisionComponent* A, UProvisionComponent* B)
{
    UProvisionComponent* ParentA = FindParent(A);
    UProvisionComponent* ParentB = FindParent(B);

    if (ParentA != ParentB)
    {
        ParentMap[ParentB] = ParentA;
    }
}

int32 UProvisionAreaManager::GetClusterIDAtLocation(const FVector& Location)
{
    for (UProvisionComponent* Comp : ProvisionComponents)
    {
        FVector CompLocation = Comp->GetComponentLocation();
        float Radius = Comp->ProvisionScore;
        float Distance = FVector::Dist(Location, CompLocation);

        if (Distance <= Radius)
        {
            return ComponentClusterMap[FindParent(Comp)];
        }
    }
    return -1; // Location is not within any provision area
}
