#pragma once
class AUnit;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnUnitResearchedNewTile, AUnit* const, const FIntPoint&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnUnitStartedWalking, AUnit* const);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnUnitFinishedWalking, AUnit* const);

class UnitDispatcher
{
public:
	template<typename UserClass>
	void BindUnitResearchedNewTile(UserClass* _pObject, void (UserClass::* _pFunc)(AUnit* const, const FIntPoint&))
	{
		m_UnitResearchedNewTileEvent.RemoveAll(_pObject);
		m_UnitResearchedNewTileEvent.AddUObject(_pObject, _pFunc);
	}

	template<typename UserClass>
	void BindUnitStartedWalking(UserClass* _pObject, void (UserClass::* _pFunc)(AUnit* const))
	{
		m_UnitStartedWalkingEvent.RemoveAll(_pObject);
		m_UnitStartedWalkingEvent.AddUObject(_pObject, _pFunc);
	}

	template<typename UserClass>
	void BindUnitFinishedWalking(UserClass* _pObject, void (UserClass::* _pFunc)(AUnit* const))
	{
		m_UnitFinishedWalkingEvent.RemoveAll(_pObject);
		m_UnitFinishedWalkingEvent.AddUObject(_pObject, _pFunc);
	}

protected:
	void BroadcastUnitResearchedNewTile(AUnit* const _pUnit, const FIntPoint& _researchedTileIndex)
	{
		m_UnitResearchedNewTileEvent.Broadcast(_pUnit, _researchedTileIndex);
	}

	void BroadcastUnitStartedWalking(AUnit* const _pUnit)
	{
		m_UnitStartedWalkingEvent.Broadcast(_pUnit);
	}

	void BroadcastUnitFinishedWalking(AUnit* const _pUnit)
	{
		m_UnitFinishedWalkingEvent.Broadcast(_pUnit);
	}
private:
	FOnUnitResearchedNewTile m_UnitResearchedNewTileEvent;
	FOnUnitStartedWalking m_UnitStartedWalkingEvent;
	FOnUnitFinishedWalking m_UnitFinishedWalkingEvent;
};

