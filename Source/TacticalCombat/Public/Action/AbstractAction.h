// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbstractAction.generated.h"

class APlayerActions;

UCLASS(Abstract)
class TACTICALCOMBAT_API UAbstractAction : public UObject
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	UAbstractAction();

public:
	virtual void BeginDestroy() override;
	
	virtual void Initialize(APlayerActions* const _pPlayerActions);
	
	// 순수 가상 함수(virtual void Execute() = 0)를 정의하면 C++에서는 추상 클래스로 인식되지만,
	// 언리얼의 GENERATED_BODY() 매크로는 일부 내부 구현을 요구하기 때문에
	// AActor 기반 클래스에서 순수 가상 함수만 남겨두면 컴파일 에러가 발생할 수 있음.
	// 해결 방법: Execute를 일반 virtual 함수로 바꾸거나 BlueprintNativeEvent로 선언
	virtual void Execute(const FIntPoint& _index) {};


protected:
	UPROPERTY(VisibleAnywhere, Category = "Internal")
	TWeakObjectPtr<APlayerActions> m_PlayerActions;

};
