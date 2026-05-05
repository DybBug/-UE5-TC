#pragma once

class ASpellAnimation;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnSpellAnimationFinished, ASpellAnimation const*);


class SpellDispatcher
{
public:
	template<typename UserClass>
	void BindSdefpellAnimationFinished(UserClass* _pUser, void (UserClass::* _pFunc)(ASpellAnimation const*) ) 
	{ 
		m_SpellAnimationFinishedEvent.RemoveAll(_pUser);
		m_SpellAnimationFinishedEvent.AddUObject(_pUser, _pFunc); 
	}

protected:
	void BroadcastSpellAnimationFinished(ASpellAnimation const* _SpellAnimation) { m_SpellAnimationFinishedEvent.Broadcast(_SpellAnimation); }

private:
	FOnSpellAnimationFinished m_SpellAnimationFinishedEvent;

};

