#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ItemInterface.generated.h"

// 인터페이스를 UObject 시스템에서 사용하기 위한 기본 매크로
UINTERFACE(MinimalAPI)
class UItemInterface : public UInterface
{
    GENERATED_BODY()
};

// 실제 C++ 레벨에서 사용할 함수 원형(시그니처)를 정의
class HW08_API IItemInterface
{
    GENERATED_BODY()

public:
    UFUNCTION()
    virtual void OnItemOverlap(
        UPrimitiveComponent* OverlappedComp,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult) = 0;
    UFUNCTION()
    virtual void OnItemEndOverlap(
        UPrimitiveComponent* OverlappedComp,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex) = 0;
    virtual void ActivateItem(AActor* Activator) = 0;
    virtual FName GetItemType() const = 0;
};