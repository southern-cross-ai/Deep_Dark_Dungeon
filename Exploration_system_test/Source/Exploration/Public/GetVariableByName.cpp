// Fill out your copyright notice in the Description page of Project Settings.


#include "GetVariableByName.h"
#include "Blueprint/UserWidget.h"     // UUserWidget
#include "Components/Widget.h"        // UWidget
#include "UObject/UnrealType.h"       // FProperty, FObjectProperty
#include "UObject/Class.h"


// Sets default values for this component's properties
UGetVariableByName::UGetVariableByName()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGetVariableByName::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UGetVariableByName::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

UFUNCTION(BlueprintCallable, Category="UI")
UWidget* UGetVariableByName::GetWidgetByName(UUserWidget* Widget, const FString& Name)
{
	if (!Widget)
	{
		UE_LOG(LogTemp, Warning, TEXT("GetWidgetByName: Widget is null"));
		return nullptr;
	}

	UClass* WidgetClass = Widget->GetClass();
	FName PropertyName(*Name);

	// 在类中查找具有该名称的属性
	FProperty* FoundProperty = WidgetClass->FindPropertyByName(PropertyName);
	if (!FoundProperty)
	{
		UE_LOG(LogTemp, Warning, TEXT("GetWidgetByName: Property '%s' not found"), *Name);
		return nullptr;
	}

	// 检查属性是否是 UObject 类型
	if (FObjectProperty* ObjProp = CastField<FObjectProperty>(FoundProperty))
	{
		if (ObjProp->PropertyClass->IsChildOf(UWidget::StaticClass()))
		{
			UObject* Obj = ObjProp->GetObjectPropertyValue_InContainer(Widget);
			return Cast<UWidget>(Obj);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("GetWidgetByName: Property '%s' is not a UWidget or not accessible"), *Name);
	return nullptr;
}

