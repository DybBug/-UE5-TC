#pragma once
class UAbstractAction;


DECLARE_MULTICAST_DELEGATE_TwoParams(FOnSelectedActionsChanged, const UAbstractAction* const, const UAbstractAction* const);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSelectedTileChanged, const FIntPoint&);


class PlayerActionDispatcher
{

public:
	template<typename UserClass>
	void BindSelectedActionsChanged(UserClass* _pObject, void (UserClass::* _pFunc)(const UAbstractAction* const, const UAbstractAction* const))
	{
		m_SelectedActionsChangedEvent.RemoveAll(_pObject);
		m_SelectedActionsChangedEvent.AddUObject(_pObject, _pFunc);
	}

	template<typename UserClass>
	void BindSelectedTileChanged(UserClass* _pObject, void (UserClass::* _pFunc)(const FIntPoint&))
	{
		m_SelectedTileChangedEvent.RemoveAll(_pObject);
		m_SelectedTileChangedEvent.AddUObject(_pObject, _pFunc);
	}

protected:
	void BroadcastSelectedActionsChanged(const UAbstractAction* const _leftClickAction, const UAbstractAction* const _rightClickAction)
	{
		m_SelectedActionsChangedEvent.Broadcast(_leftClickAction, _rightClickAction);
	}

	void BroadcastSelectedTileChanged(const FIntPoint& _index)
	{
		m_SelectedTileChangedEvent.Broadcast(_index);
	}

private:
	FOnSelectedActionsChanged m_SelectedActionsChangedEvent;
	FOnSelectedTileChanged m_SelectedTileChangedEvent;
};

