#pragma once
// Delegate 타입
DECLARE_MULTICAST_DELEGATE_OneParam(FOnTileDataChanged, const FIntPoint&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnTileStateChanged, const FIntPoint&);
DECLARE_MULTICAST_DELEGATE(FOnGridCreated);
DECLARE_MULTICAST_DELEGATE(FOnGridDestroyed);

class GridDispatcher
{
public:
    // 바인딩용 Getter
    template<typename UserClass>
    void BindTileDataChanged(UserClass* _pObject, void (UserClass::* _pFunc)(const FIntPoint&))
    { 
		m_TileDataChangedEvent.RemoveAll(_pObject);
        m_TileDataChangedEvent.AddUObject(_pObject, _pFunc); 
    }

    template<typename UserClass>
    void BindTileStateChanged(UserClass* _pObject, void (UserClass::* _pFunc)(const FIntPoint&)) 
    { 
		m_TileStateChangedEvent.RemoveAll(_pObject);
        m_TileStateChangedEvent.AddUObject(_pObject, _pFunc); 
    }

    template<typename UserClass>
    void BindGridCreated(UserClass* _pObject, void (UserClass::* _pFunc)()) 
    { 
		m_GridCreatedEvent.RemoveAll(_pObject);
        m_GridCreatedEvent.AddUObject(_pObject, _pFunc); 
    }

    template<typename UserClass>
    void BindGridDestroyed(UserClass* _pObject, void (UserClass::* _pFunc)()) 
    { 
		m_GridDestroyedEvent.RemoveAll(_pObject);
        m_GridDestroyedEvent.AddUObject(_pObject, _pFunc); 
    }

protected:
    // 브로드캐스트 함수
    void BroadcastTileDataChanged(const FIntPoint& Point) { m_TileDataChangedEvent.Broadcast(Point); }
    void BroadcastTileStateChanged(const FIntPoint& Point) { m_TileStateChangedEvent.Broadcast(Point); }
    void BroadcastGridCreated() {  m_GridCreatedEvent.Broadcast(); }
    void BroadcastGridDestroyed() { m_GridDestroyedEvent.Broadcast(); }

private:
    // 델리게이트 인스턴스
    FOnTileDataChanged m_TileDataChangedEvent;
    FOnTileStateChanged m_TileStateChangedEvent;
    FOnGridCreated m_GridCreatedEvent;
    FOnGridDestroyed m_GridDestroyedEvent;
};