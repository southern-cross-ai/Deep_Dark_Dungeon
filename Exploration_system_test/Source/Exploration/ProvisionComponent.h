#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "ProvisionComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EXPLORATION_API UProvisionComponent : public USceneComponent
{
    GENERATED_BODY()

public:    
    UProvisionComponent();

protected:
    virtual void BeginPlay() override;

#if WITH_EDITOR
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

public:    
    // Provision Score that defines the radius
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Provision")
    float ProvisionScore;

    // Sphere component for visualization   
    UPROPERTY(VisibleAnywhere, Category="Provision")
    class USphereComponent* ProvisionSphere;

    // Static Mesh component for enhanced visualization
    UPROPERTY(VisibleAnywhere, Category="Provision")
    class UStaticMeshComponent* ProvisionCircleMesh;

    // Material for the static mesh
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Provision")
    class UMaterialInterface* ProvisionMaterial;

    // Setter function for ProvisionScore
    UFUNCTION(BlueprintCallable, Category="Provision")
    void SetProvisionScore(float NewScore);
};
