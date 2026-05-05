
#pragma once


DECLARE_MULTICAST_DELEGATE(FOnLevelLoaded)
/**
 * 
 */
class TACTICALCOMBAT_API LevelLoadingDispatcher
{

public:
	template<typename UserClass>
	void BindLevelLoaded(UserClass* _pObject, void (UserClass::* _pFunc)()) 
	{ 
		m_LevelLoadedEvent.RemoveAll(_pObject);
		m_LevelLoadedEvent.AddUObject(_pObject, _pFunc); 
	}

protected:
	void BroadcastLevelLoaded() { m_LevelLoadedEvent.Broadcast(); }

private:
	FOnLevelLoaded m_LevelLoadedEvent;
};
