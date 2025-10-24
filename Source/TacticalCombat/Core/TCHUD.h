// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TCHUD.generated.h"

class UDebugMenu;
class UDebugTextOnTiles;
/**
 * 
 */
UCLASS()
class TACTICALCOMBAT_API ATCHUD : public AHUD
{
	GENERATED_BODY()

public:
	ATCHUD();
	
protected:
	virtual void BeginPlay() override;
	
protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UDebugMenu> m_DebugMenuClass;
	
	UPROPERTY()
	TObjectPtr<UDebugMenu> m_DebugMenu;
};
