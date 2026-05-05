#pragma once

DECLARE_MULTICAST_DELEGATE(FOnAnimatedShapeFinished)

class AnimatedShapeDispatcher
{
public:
	template<typename UserClass>
	void BindAnimatedShapeFinished(UserClass* _pObject, void (UserClass::* _pFunc)()) 
	{
		m_AnimatedShapeFinishedEvent.RemoveAll(_pObject);
		m_AnimatedShapeFinishedEvent.AddUObject(_pObject, _pFunc); 
	}

protected:
	void BroadcastAnimatedShapeFinished() { m_AnimatedShapeFinishedEvent.Broadcast(); }

private:
	FOnAnimatedShapeFinished m_AnimatedShapeFinishedEvent;
};

