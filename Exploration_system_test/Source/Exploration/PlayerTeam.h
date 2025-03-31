// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseHero.h"
#include "PlayerTeam.generated.h"

UCLASS(Blueprintable)
class APlayerTeam : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerTeam();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
    
	// Maximum Movement Points
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    int32 MaxMovementPoints;

    // Current Movement Points
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    int32 CurrentMovementPoints;

    // Regeneration Rate (Movement Points per second)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float MovementPointRegenRate;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team")
	TArray<ABaseHero*> TeamHeroes;
	virtual void BeginPlay() override;
private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;


	    // Timer handle for regeneration
    FTimerHandle MovementPointRegenTimerHandle;

    // Regenerate Movement Points
    void RegenerateMovementPoints();
};