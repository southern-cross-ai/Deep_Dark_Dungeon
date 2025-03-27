#include "JsonParserUtility.h"
#include "Engine/Engine.h" 

TArray<TSharedPtr<FJsonObject>> JsonParserUtility::ParseEventFromJSON(const FString& JSONString)
{
    TArray<TSharedPtr<FJsonObject>> ParserResults;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JSONString);

    // key:value
    TSharedPtr<FJsonObject> JSONObject;


    if (FJsonSerializer::Deserialize(Reader, JSONObject) && JSONObject.IsValid())
    {

        const TArray<TSharedPtr<FJsonValue>>* EventsArray;
        if (JSONObject->TryGetArrayField(TEXT("events"), EventsArray))
        {

            for (const auto& EventValue : *EventsArray)
            {
                const TSharedPtr<FJsonObject> EventObject = EventValue->AsObject();
                if (EventObject.IsValid())
                {
                    ParserResults.Add(EventObject);
                }
            }
        }
    }
    return ParserResults;
}