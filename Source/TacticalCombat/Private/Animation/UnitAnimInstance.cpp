// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/UnitAnimInstance.h"
#include "Shared/SharedEnums.h"

void UUnitAnimInstance::SetUnitAnimationState_Implementation(EUnitAnimationState _newState)
{
	UWorld* pWorld = GetWorld();
	if (!pWorld) return;

	if (m_UnitAnimationState == _newState)
	{
		m_UnitAnimationState = EUnitAnimationState::Idle;
		FTimerDelegate timerDelegate;
		timerDelegate.BindUObject(this, &UUnitAnimInstance::_ApplyUnitAnimationState, _newState);

		pWorld->GetTimerManager().ClearTimer(m_hStateUpdateTimer);
		m_hStateUpdateTimer.Invalidate();
		pWorld->GetTimerManager().SetTimer(m_hStateUpdateTimer, timerDelegate, 0.001f, false);		
	}
	else
	{
		_ApplyUnitAnimationState(_newState);
	}	
}

void UUnitAnimInstance::_ApplyUnitAnimationState(EUnitAnimationState _newState)
{
	UWorld* pWorld = GetWorld();
	if (!pWorld) return;
	// 1. 상태를 먼저 변경합니다.
	m_UnitAnimationState = _newState;

	// 3. 상태별 대기 시간 설정
	float delayTime = 0.0f;
	switch (_newState)
	{
		case EUnitAnimationState::Attack:
		case EUnitAnimationState::Hit:
			delayTime = 0.5f;
		break;
		case EUnitAnimationState::Respawn:
			delayTime = 1.0f;
		break;
		case EUnitAnimationState::Idle:
		case EUnitAnimationState::Walk:
		default:
			return;
	}
	
	// 2. 기존 타이머가 있다면 취소합니다 (리트리거 효과)
	pWorld->GetTimerManager().ClearTimer(m_hStateBackToIdleTimer);

	// 4. 설정된 시간 후에 BackToIdle 호출
	if (delayTime > 0.0f)
	{
		FTimerDelegate timerDelegate;
		timerDelegate.BindUObject(this, &UUnitAnimInstance::_ResetToIdleState);
		pWorld->GetTimerManager().ClearTimer(m_hStateBackToIdleTimer);
		m_hStateUpdateTimer.Invalidate();
		pWorld->GetTimerManager().SetTimer(m_hStateBackToIdleTimer, timerDelegate, delayTime, false);
	}
}

void UUnitAnimInstance::_ResetToIdleState()
{
	m_UnitAnimationState = EUnitAnimationState::Idle;
}
