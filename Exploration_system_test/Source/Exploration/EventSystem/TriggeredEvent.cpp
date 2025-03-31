// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggeredEvent.h"
#include "Components/ActorComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EventUI/MyUserWidget.h"
#include "Exploration/ExplorationPlayerController.h"

TArray<UActivity*> UTriggeredEvent::CurEvents;

void UTriggeredEvent::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Log, TEXT("Handling event"));
	EventDisplay();
}

void UTriggeredEvent::BeginPlay() {
	Super::BeginPlay();
	if (AActor* Owner = GetOwner()) {
		if (isOverlapType) {
			StaticMeshComponent = Owner->FindComponentByClass<UStaticMeshComponent>();
			if (StaticMeshComponent != nullptr) {
				StaticMeshComponent->SetCollisionProfileName(TEXT("trigger"));
				StaticMeshComponent->SetGenerateOverlapEvents(true);

				StaticMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &UTriggeredEvent::OnOverlapBegin);
			}
		}
		
	}
	
}

void UTriggeredEvent::CheckTrigger() {
    if (isChildEvent) {
        IfTriggerable = true;
        return;
    }
	if (!isOverlapType) {
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (PlayerController) {
			APawn* PlayerPawn = PlayerController->GetPawn();
			if (PlayerPawn) {
				FVector PlayerLocation = PlayerPawn->GetActorLocation();
				if (AActor* Owner = GetOwner()) {
					double distance = FVector::Dist(PlayerLocation, Owner->GetActorLocation());
					UE_LOG(LogTemp, Log, TEXT("distance:%f"),distance);
					if (distance < 400) {
						IfTriggerable = true;
					}
				}
				else {
					IfTriggerable = false;
				}
			}
			else {
				IfTriggerable = false;
			}

		}

	}
	else {
		IfTriggerable = true;
	}
}

void UTriggeredEvent::HandleNextEvent() {
    if (EventsArray.IsValidIndex(CurrentIndex))
    {
        TSharedPtr<FJsonObject> CurrentEvent = EventsArray[CurrentIndex];
        FString EventType = CurrentEvent->GetStringField(TEXT("type"));
        FString Description = CurrentEvent->GetStringField(TEXT("description"));

        // 根据事件类型显示不同内容
        if (EventType == "dialogue")
        {
            const TSharedPtr<FJsonObject> CharacterObject = CurrentEvent->GetObjectField(TEXT("character"));
            FString CharacterName = CharacterObject->GetStringField(TEXT("name"));
            FString PortraitURL = CharacterObject->GetStringField(TEXT("portrait_url"));
            UE_LOG(LogTemp, Log, TEXT("Dialogue: %s, Character: %s, Portrait: %s"), *Description, *CharacterName, *PortraitURL);
            if (Widget != nullptr) {
                Widget->RemoveFromParent();
            }
            Widget = CreateWidget<UMyUserWidget>(GetWorld(), LoadClass<UMyUserWidget>(nullptr, TEXT("/Game/UI/WBP_Dialogue.WBP_Dialogue_C")));
            Widget->InitializeComponentWithInput(*Description);
            Widget->InitializeComponentWithInput2(*CharacterName);
            Widget->InitializeComponentWithInput5(*PortraitURL);
            Widget->AddToViewport();
        }
        else if (EventType == "event")
        {
            if (Widget != nullptr) {
                Widget->RemoveFromParent();
            }
            Widget = CreateWidget<UMyUserWidget>(GetWorld(), LoadClass<UMyUserWidget>(nullptr, TEXT("/Game/UI/WBP_Dialogue.WBP_Dialogue_C")));
            Widget->InitializeComponentWithInput(*Description);
            Widget->InitializeComponentWithInput2(TEXT("Voice-over"));
            Widget->AddToViewport();
            UE_LOG(LogTemp, Log, TEXT("Event: %s"), *Description);
        }
        else if (EventType == "option")
        {
            UE_LOG(LogTemp, Log, TEXT("Option: %s"), *Description);
            const TArray<TSharedPtr<FJsonValue>>* OptionsArray;
            if (CurrentEvent->TryGetArrayField(TEXT("options"), OptionsArray))
            {
                int count = 0;
                int length = OptionsArray->Num();
                AEventVector* EventVector = AEventVector::FindEventManager(GetWorld());
                TArray<FString> DesArray;
                TArray<float> SucArray;
                DesArray.SetNum(length);
                SucArray.SetNum(length);
                OptionList.SetNum(length);
                CurEvents.Empty();
                for (const auto& OptionValue : *OptionsArray)
                {
                    const TSharedPtr<FJsonObject> OptionObject = OptionValue->AsObject();
                    FString OptionText = OptionObject->GetStringField(TEXT("option_text"));
                    FString NextEventID = OptionObject->GetStringField(TEXT("next_event_id"));

                    OptionList[count] = NewObject<UOption>(); 
                    UActivity* CurEvent = EventVector->GetEvent(NextEventID);
                    OptionList[count]->OptionEvent = CurEvent;
                    CurEvents.Add(CurEvent);
                    DesArray[count] = OptionText;
                    if (OptionList[count] == nullptr)
                    {
                        UE_LOG(LogTemp, Warning, TEXT("OptionList[%d] is null!"), count);
                    }
                    else
                    {
                        SucArray[count] = OptionList[count]->RateCalculator();
                        UE_LOG(LogTemp, Log, TEXT("RateCalculator executed for OptionList[%d]"), count);
                    }
                    //SucArray[count] = OptionList[count]->RateCalculator();
                    UE_LOG(LogTemp, Log, TEXT("Option Text: %s, Next Event ID: %s"), *OptionText, *NextEventID);
                    count++;
                }
                if (Widget != nullptr) {
                    Widget->RemoveFromParent();
                }
                if (CurEvents.IsEmpty()) {
                    UE_LOG(LogTemp, Log, TEXT("Empty warning!!!"));
                }
                UnbindMouseInput();
                Widget = CreateWidget<UMyUserWidget>(GetWorld(), LoadClass<UMyUserWidget>(nullptr, TEXT("/Game/UI/WBP_ChoiceDialogue.WBP_ChoiceDialogue_C")));
                Widget->InitializeComponentWithInput(*Description);
                Widget->InitializeComponentWithInput2(TEXT("Voice-over"));
                Widget->InitializeComponentWithInput3(DesArray);
                Widget->InitializeComponentWithInput4(SucArray);
                Widget->AddToViewport();
            }
        }

        // 更新当前索引
        CurrentIndex++;
    }
    else
    {
        if (Widget != nullptr) {
            Widget->RemoveFromParent();
        }
        UpdateState();
        UpdateEventCount();
        UnbindMouseInput();
        APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
        AExplorationPlayerController* ExplorationPlayerController = Cast<AExplorationPlayerController>(PlayerController);
        ExplorationPlayerController->SetupInputComponent();
    }
}