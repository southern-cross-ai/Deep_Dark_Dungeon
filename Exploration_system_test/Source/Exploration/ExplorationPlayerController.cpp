// Copyright Epic Games, Inc. All Rights Reserved.

#include "ExplorationPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "EventSystem/TriggeredEvent.h"
#include "ExplorationCharacter.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "TeamEventComponent.h"
#include "ProvisionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Site.h"
#include "PlayerTeam.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

AExplorationPlayerController::AExplorationPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;
    MovementPointConsumptionRate = 1.0f; // MovementPoints consumed per unit distance
}

void AExplorationPlayerController::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	
	// Initialize the ProvisionAreaManager
    ProvisionAreaManager = NewObject<UProvisionAreaManager>(this);
    ProvisionAreaManager->Initialize();
}

void AExplorationPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Setup mouse input events
		//EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Started, this, &AExplorationPlayerController::OnInputStarted);
		//EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Triggered, this, &AExplorationPlayerController::OnSetDestinationTriggered);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Completed, this, &AExplorationPlayerController::OnSetDestinationReleased);
		//EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Canceled, this, &AExplorationPlayerController::OnSetDestinationReleased);

		// Setup touch input events
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Started, this, &AExplorationPlayerController::OnInputStarted);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Triggered, this, &AExplorationPlayerController::OnTouchTriggered);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Completed, this, &AExplorationPlayerController::OnTouchReleased);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Canceled, this, &AExplorationPlayerController::OnTouchReleased);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AExplorationPlayerController::UnBindMouseClick() {
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer())){ 
		Subsystem->RemoveMappingContext(DefaultMappingContext);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("Failed to find Subsystem!"));
	}
}

//void AExplorationPlayerController::ReBindMouseClick() {
//	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent)) {
//		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Completed, this, &AExplorationPlayerController::OnSetDestinationReleased);
//	}
//	else
//	{
//		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
//	}
//}

void AExplorationPlayerController::OnInputStarted()
{
	StopMovement();
}

// Triggered every frame when the input is held down
void AExplorationPlayerController::OnSetDestinationTriggered()
{
    // We flag that the input is being pressed
    FollowTime += GetWorld()->GetDeltaSeconds();
    
    // We look for the location in the world where the player has pressed the input
    FHitResult Hit;
    bool bHitSuccessful = false;
    if (bIsTouch)
    {
        bHitSuccessful = GetHitResultUnderFinger(ETouchIndex::Touch1, ECollisionChannel::ECC_Visibility, true, Hit);
    }
    else
    {
        bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);
    }

	auto name = Hit.GetActor()->GetFName().ToString();
	UE_LOG(LogTemp, Display, TEXT("Actor Name: %s"), *name);


    // If we hit a surface, cache the location
    if (bHitSuccessful)
    {
        CachedDestination = Hit.Location;
    }
    
    // Move towards mouse pointer or touch
    APawn* ControlledPawn = GetPawn();
    if (ControlledPawn != nullptr)
    {
        // **Add the check here**
        if (CanMoveToLocation(CachedDestination))
        {
            if (ControlledPawn != nullptr)
            {
                float Distance = FVector::Dist(ControlledPawn->GetActorLocation(), CachedDestination);
                if (HasEnoughMovementPoints(Distance))
                {
                    FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
                    ControlledPawn->AddMovementInput(WorldDirection, 1.0, false);
                    ConsumeMovementPoints(Distance * GetWorld()->GetDeltaSeconds());
                }
                else
                {
                    UE_LOG(LogTemp, Warning, TEXT("Not enough MovementPoints to move."));
                    StopMovement();
                }
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Cannot move to location outside of provision areas."));
            StopMovement();
        }
    }
}

void AExplorationPlayerController::OnSetDestinationReleased()
{
    FHitResult Hit;
    bool bHitSuccessful = false;
    bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel1, true, Hit);

    // Event Handle here
    if (bHitSuccessful)
    {
        AActor* HitActor = Hit.GetActor();
        if (HitActor)
        {
            UTeamEventComponent* EventComponent = HitActor->FindComponentByClass<UTeamEventComponent>();

            if (EventComponent) {
                EventComponent->HandleEvent();
            }

            UTriggeredEvent* TriggeredEvent = HitActor->FindComponentByClass<UTriggeredEvent>();
            if (TriggeredEvent&&!TriggeredEvent->isOverlapType) {
                TriggeredEvent->EventDisplay();
            }
            
        }

        return;
    }


	bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);
    if (bHitSuccessful)
    {
        FVector TargetLocation = Hit.Location;

        // **Use the new function to check if movement is allowed**
        if (CanMoveToLocation(TargetLocation))
        {
            APawn* ControlledPawn = GetPawn();
            // Calculate distance to target
            if (ControlledPawn != nullptr)
            {
                float Distance = FVector::Dist(ControlledPawn->GetActorLocation(), TargetLocation);

                // Check if the team has enough MovementPoints
                if (HasEnoughMovementPoints(Distance))
                {
                    // Consume MovementPoints
                    ConsumeMovementPoints(Distance);

                    // Move the pawn
                    UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, TargetLocation);
                }
                else
                {
                    UE_LOG(LogTemp, Warning, TEXT("Not enough MovementPoints to move."));
                    // Optionally, provide feedback to the player here
                }
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Cannot move to location outside of connected provision areas."));
            // Optionally, provide feedback to the player here
        }
    }

	// If it was a short press
	//if (FollowTime <= ShortPressThreshold)
	{
		// We move there and spawn some particles
		//UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, CachedDestination);
		//UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, CachedDestination, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
	}

	//FollowTime = 0.f;
}

// Triggered every frame when the input is held down
void AExplorationPlayerController::OnTouchTriggered()
{
    bIsTouch = true;
    OnSetDestinationTriggered();
}

void AExplorationPlayerController::OnTouchReleased()
{
    bIsTouch = false;
    OnSetDestinationReleased();
}



//Provision related

bool AExplorationPlayerController::CanMoveToLocation(const FVector& TargetLocation)
{
    if (!ProvisionAreaManager)
    {
        // If the manager is not available, allow movement
        return true;
    }

    // Get cluster IDs for current position and target location
    int32 CurrentClusterID = ProvisionAreaManager->GetClusterIDAtLocation(GetPawn()->GetActorLocation());
    int32 TargetClusterID = ProvisionAreaManager->GetClusterIDAtLocation(TargetLocation);

    if (CurrentClusterID == -1 || TargetClusterID == -1)
    {
        // Either current position or target location is not within any provision area
        return false;
    }

    // Allow movement only if both are in the same cluster
    return CurrentClusterID == TargetClusterID;
}


bool AExplorationPlayerController::HasEnoughMovementPoints(float Distance)
{
    APlayerTeam* PlayerTeam = Cast<APlayerTeam>(GetPawn());
    if (PlayerTeam)
    {
        float RequiredPoints = Distance * MovementPointConsumptionRate;
        return PlayerTeam->CurrentMovementPoints >= RequiredPoints;
    }
    return false;
}

void AExplorationPlayerController::ConsumeMovementPoints(float Distance)
{
    APlayerTeam* PlayerTeam = Cast<APlayerTeam>(GetPawn());
    if (PlayerTeam)
    {
        int32 PointsToConsume = FMath::CeilToInt(Distance * MovementPointConsumptionRate);
        PlayerTeam->CurrentMovementPoints -= PointsToConsume;
        PlayerTeam->CurrentMovementPoints = FMath::Clamp(PlayerTeam->CurrentMovementPoints, 0, PlayerTeam->MaxMovementPoints);
    }
}