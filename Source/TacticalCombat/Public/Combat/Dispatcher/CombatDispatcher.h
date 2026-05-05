
#pragma once
class AUnit;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnUnitGridIndexChanged, AUnit* const)


class CombatDispatcher
{
public:
	template<typename UserClass>
	void BindUnitGridIndexChanged(UserClass* _pObject, void (UserClass::* _pFunc)(AUnit* const)) 
	{ 
		m_UnitGridIndexChangedEvent.RemoveAll(_pObject);
		m_UnitGridIndexChangedEvent.AddUObject(_pObject, _pFunc); 
	}

protected:
	void BroadcastUnitGridIndexChanged(AUnit* const _pUnit) { m_UnitGridIndexChangedEvent.Broadcast(_pUnit); }

private:
	FOnUnitGridIndexChanged m_UnitGridIndexChangedEvent;
};

