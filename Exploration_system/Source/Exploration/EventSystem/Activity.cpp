// Fill out your copyright notice in the Description page of Project Settings.


#include "Activity.h"
#include "JsonParserUtility.h"
#include "Blueprint/UserWidget.h"
#include "EventUI/MyUserWidget.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "../ExplorationPlayerController.h"


// Sets default values for this component's properties
UActivity::UActivity()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;
    // OptionList = nullptr;
    EventCount = 0;
    CurrentEventState = EEventState::NotOccurred;
    IfTriggerable = false;


    // ...
}


// Called when the game starts
void UActivity::BeginPlay()
{
    Super::BeginPlay();
    OnConditionalEventTriggered.AddDynamic(this, &UActivity::HandleNextEvent);

    //UE_LOG(LogTemp, Log, TEXT("开始"));
    //EventDisplay();
    // ...

}


// Called every frame
void UActivity::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    CheckTrigger();
    //if (IfTriggerable == true){
    //	EventDisplay();
    //}

    // ...
}

void UActivity::EventDisplay()
{
    if (!InternalCheck())
    {
        UE_LOG(LogTemp, Warning, TEXT("Event Check Error!!!"));
        return;
    }
    CheckTrigger();
    if (CurrentEventState == EEventState::NotOccurred&&IfTriggerable) {
        FString JSONString = LoadJsonFromFile();
        if (JSONString == "") {
            return;
        }
        EventsArray = JsonParserUtility::ParseEventFromJSON(JSONString);
        if (EventsArray.Num() <= 0) {
            return;
        }
        UpdateState();
        APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
        AExplorationPlayerController* ExplorationPlayerController = Cast<AExplorationPlayerController>(PlayerController);
        ExplorationPlayerController->UnBindMouseClick();
        OnConditionalEventTriggered.Broadcast();
        BindMouseInput();
    }

    //Specific Content;
    //return;
}

void UActivity::CheckTrigger()
{
    IfTriggerable = true;
}

void UActivity::UpdateEventCount()
{
    AddEventCount();
}

void UActivity::AddEventCount()
{
    EventCount += 1;
}

void UActivity::MinusEventCount()
{
    EventCount -= 1;
}

void UActivity::SetEventCount(int number)
{
    EventCount = number;
}

void UActivity::UpdateState()
{
    if (CurrentEventState == EEventState::NotOccurred)
    {
        CurrentEventState = EEventState::IsOccurring;
    }
    else if (CurrentEventState == EEventState::IsOccurring)
    {
        CurrentEventState = EEventState::HasOccurred;
    }
}

// void UpdateEnable()
// {
// 	return;
// }

bool UActivity::InternalCheck()
{

    if (CurrentEventState == EEventState::HasOccurred && EventCount == 0)
    {
        return false;
    }
    else if (EventCount > 0)
    {
        if (CurrentEventState == EEventState::NotOccurred || CurrentEventState == EEventState::IsOccurring)
            return false;
    }
    return true;
}




void UActivity::OnScreenClicked()
{
    if (CurrentEventState == EEventState::IsOccurring) {
        OnConditionalEventTriggered.Broadcast();
    }

}


void UActivity::HandleNextEvent()
{

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
            Widget = CreateWidget<UMyUserWidget>(GetWorld(), LoadClass<UMyUserWidget>(nullptr, TEXT("/Game/UI/Test2.Test2_C")));
            Widget->InitializeComponentWithInput(*Description);
            Widget->InitializeComponentWithInput2(*CharacterName);
            Widget->AddToViewport();
        }
        else if (EventType == "event")
        {
            if (Widget != nullptr) {
                Widget->RemoveFromParent();
            }
            Widget = CreateWidget<UMyUserWidget>(GetWorld(), LoadClass<UMyUserWidget>(nullptr, TEXT("/Game/UI/Test2.Test2_C")));
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
                for (const auto& OptionValue : *OptionsArray)
                {
                    const TSharedPtr<FJsonObject> OptionObject = OptionValue->AsObject();
                    FString OptionText = OptionObject->GetStringField(TEXT("option_text"));
                    int32 NextEventID = OptionObject->GetIntegerField(TEXT("next_event_id"));
                    UE_LOG(LogTemp, Log, TEXT("Option Text: %s, Next Event ID: %d"), *OptionText, NextEventID);
                }

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

void UActivity::BindMouseInput()
{
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (PlayerController && PlayerController->InputComponent)
    {
        PlayerController->InputComponent->BindAction("Click", IE_Pressed, this, &UActivity::OnScreenClicked);
        UE_LOG(LogTemp, Log, TEXT("Binding Click Action"));

    }
}

void UActivity::UnbindMouseInput()
{
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (PlayerController && PlayerController->InputComponent)
    {
        PlayerController->InputComponent->RemoveActionBinding("Click", IE_Pressed);
        UE_LOG(LogTemp, Log, TEXT("Unbounded Click Action"));
    }
}


FString UActivity::LoadJsonFromFile()
{
    FString JsonContent;
    // 检查是否提供了有效的文件路径
    if (JsonFilePath.FilePath.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("No JSON path!"));
        return JsonContent;
    }

    // 获取完整的文件路径。FPaths::ProjectContentDir() 提供了 "Content/" 目录的路径
    FString FullFilePath = FPaths::ProjectContentDir() + JsonFilePath.FilePath;

    // 使用 FFileHelper 来读取 JSON 文件
    if (FFileHelper::LoadFileToString(JsonContent, *FullFilePath))
    {
        UE_LOG(LogTemp, Log, TEXT("readed JSON file: %s"), *FullFilePath);
        return JsonContent;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("failed to read JSON file: %s"), *FullFilePath);
        return JsonContent;
    }
}

