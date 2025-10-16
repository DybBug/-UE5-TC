// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerActions.h"

#include "Actions/AbstractAction.h"
#include "Grid/Grid.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APlayerActions::APlayerActions()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<UAbstractAction> leftClickSelectActionClassFinder(TEXT("/Script/TacticalCombat.SelectTileAction"));
	if (leftClickSelectActionClassFinder.Succeeded())
	{
		m_LeftClickSelectActionClass = leftClickSelectActionClassFinder.Class;
	}

	static ConstructorHelpers::FClassFinder<UAbstractAction> rightClickSelectActionClassFinder(TEXT("/Script/TacticalCombat.SelectTileAction"));
	if (rightClickSelectActionClassFinder.Succeeded())
	{
		m_RightClickSelectActionClass = rightClickSelectActionClassFinder.Class;
	}
}

// Called when the game starts or when spawned
void APlayerActions::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* pPlayerController =  GetWorld()->GetFirstPlayerController();
	if (pPlayerController)
	{
		EnableInput(pPlayerController);
		InputComponent->BindAction("LeftClick", IE_Pressed, this, &APlayerActions::_OnLeftClicked);
		InputComponent->BindAction("RightClick", IE_Pressed, this, &APlayerActions::_OnRightClicked);
	}
	
	m_Grid = Cast<AGrid>(UGameplayStatics::GetActorOfClass(GetWorld(), AGrid::StaticClass()));
}

// Called every frame
void APlayerActions::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	_UpdateHoveredTile();
}

void APlayerActions::SetSelectedActionWithNotify(const TSubclassOf<UAbstractAction>& _leftClickActionClass, const TSubclassOf<UAbstractAction>& _rightClickActionClass)
{
	if (m_LeftClickSelectAction)
	{		
		m_LeftClickSelectAction->ConditionalBeginDestroy(); // 오브젝트 삭제
		m_LeftClickSelectAction = nullptr;
	}
	if (_leftClickActionClass != nullptr)
	{		
		m_LeftClickSelectAction = NewObject<UAbstractAction>(this, _leftClickActionClass);
		m_LeftClickSelectAction->Initialize(this);
	}

	if (m_RightClickSelectAction)
	{
		m_RightClickSelectAction->ConditionalBeginDestroy();// 오브젝트 삭제
		m_RightClickSelectAction = nullptr;
	}

	if (_rightClickActionClass != nullptr)
	{
		m_RightClickSelectAction = NewObject<UAbstractAction>(this, _rightClickActionClass);
		m_RightClickSelectAction->Initialize(this);
	}

	if (OnSelectedActionsChanged.IsBound())
	{
		OnSelectedActionsChanged.Broadcast(m_LeftClickSelectAction, m_RightClickSelectAction);
	}
}

void APlayerActions::_UpdateHoveredTile()
{
	const FIntPoint& hoveredTileIndex =  m_Grid->GetTileIndexUnderCursor(0);
	if (m_HoveredTileIndex != hoveredTileIndex)
	{
		m_Grid->RemoveStateToTile(m_HoveredTileIndex, ETileStateFlags::Hovered);
		
		m_HoveredTileIndex = hoveredTileIndex;
		m_Grid->AddStateToTile(m_HoveredTileIndex, ETileStateFlags::Hovered);
	}
}

void APlayerActions::_OnLeftClicked()
{
	_UpdateHoveredTile();
	if (m_LeftClickSelectAction)
	{
		m_LeftClickSelectAction->Execute(m_HoveredTileIndex);
	}
}

void APlayerActions::_OnRightClicked()
{
	_UpdateHoveredTile();
	if (m_RightClickSelectAction)
	{
		m_RightClickSelectAction->Execute(m_HoveredTileIndex);
	}
}


