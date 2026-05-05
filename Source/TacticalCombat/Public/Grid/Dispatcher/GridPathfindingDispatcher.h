#pragma once

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPathfindingNodeUpdated, const FIntPoint&);
DECLARE_MULTICAST_DELEGATE(FOnPathfindingNodeCleared);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPathfindingCompleted, const TArray<FIntPoint>&);

class GridPathfindingDispatcher
{
public:
	template<typename UserClass>
	void BindPathfindingNodeUpdated(UserClass* _pObject, void (UserClass::*_pFunc)(const FIntPoint&)) 
	{
		m_PathfindingNodeUpdatedEvent.RemoveAll(_pObject);
		m_PathfindingNodeUpdatedEvent.AddUObject(_pObject, _pFunc);
	}

	template<typename UserClass>
	void BindPathfindingNodeCleared(UserClass* _pObject, void (UserClass::* _pFunc)())
	{
		m_PathfindingNodeClearedEvent.RemoveAll(_pObject);
		m_PathfindingNodeClearedEvent.AddUObject(_pObject, _pFunc);
	}

	template<typename UserClass>
	void BindPathfindingCompleted(UserClass* _pObject, void (UserClass::* _pFunc)(const TArray<FIntPoint>&))
	{
		m_PathfindingCompletedEvent.RemoveAll(_pObject);
		m_PathfindingCompletedEvent.AddUObject(_pObject, _pFunc);
	}

protected:
	void BroadcastPathfindingNodeUpdated(const FIntPoint& _index) { m_PathfindingNodeUpdatedEvent.Broadcast(_index); }
	void BroadcastPathfindingNodeCleared() { m_PathfindingNodeClearedEvent.Broadcast(); }
	void BroadcastPathfindingCompleted(const TArray<FIntPoint>& _path) { m_PathfindingCompletedEvent.Broadcast(_path); }

private:
	FOnPathfindingNodeUpdated m_PathfindingNodeUpdatedEvent;
	FOnPathfindingNodeCleared m_PathfindingNodeClearedEvent;
	FOnPathfindingCompleted m_PathfindingCompletedEvent;

};

