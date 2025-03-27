#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Site.generated.h"

UCLASS()
class EXPLORATION_API ASite : public AActor
{
    GENERATED_BODY()
    
public:    
    // Sets default values for this actor's properties
    ASite();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:    
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Static mesh component for the site
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UStaticMeshComponent* MeshComponent;

};
