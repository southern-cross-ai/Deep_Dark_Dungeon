#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ProvisionComponent.h"
#include "ProvisionAreaManager.generated.h"

UCLASS()
class EXPLORATION_API UProvisionAreaManager : public UObject
{
    GENERATED_BODY()
    
public:
    UProvisionAreaManager(const FObjectInitializer& ObjectInitializer);

    // Initialize and compute clusters
    void Initialize();

    // Get the cluster ID for a given location
    int32 GetClusterIDAtLocation(const FVector& Location);

private:
    UWorld* World;

    // Array of all provision components
    TArray<UProvisionComponent*> ProvisionComponents;

    // Mapping from provision component to cluster ID
    TMap<UProvisionComponent*, int32> ComponentClusterMap;

    // Union-Find data structure
    TMap<UProvisionComponent*, UProvisionComponent*> ParentMap;

    // Union-Find functions
    UProvisionComponent* FindParent(UProvisionComponent* Component);
    void UnionComponents(UProvisionComponent* A, UProvisionComponent* B);
};
