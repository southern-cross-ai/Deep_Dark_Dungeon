#pragma once

#include "CoreMinimal.h"
#include "Json.h"
#include "JsonUtilities.h"

class EXPLORATION_API JsonParserUtility
{
public:
    static TArray<TSharedPtr<FJsonObject>> ParseEventFromJSON(const FString& JSONString);
};