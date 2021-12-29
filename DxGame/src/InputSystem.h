#pragma once
#include <set>

class IInputListener;

class InputSystem
{
public:
	virtual ~InputSystem();
	static InputSystem* Instance();
	void Update();
	void AddListener(IInputListener* p_listener);
	void RemoveListener(IInputListener* p_listener);
private:
	InputSystem();
	void Notify(byte key, bool is_down);
protected:
	byte m_key_status[256];
	byte m_old_key_status[256];
	std::set<IInputListener*> m_listeners;
};

