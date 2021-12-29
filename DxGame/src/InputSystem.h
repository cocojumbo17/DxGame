#pragma once
#include <set>
#include "Point2d.h"


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
	void KeyboardHandle();
	void MouseHandle();
protected:
	byte m_key_status[256];
	byte m_old_key_status[256];
	std::set<IInputListener*> m_listeners;
	Point2d m_cur_mouse_pos;
	Point2d m_old_mouse_pos;
};

