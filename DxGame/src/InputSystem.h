#pragma once
#include <set>
#include "Point2d.h"


class IInputListener;

class InputSystem
{
public:
	static void Create();
	static void Release();
	static InputSystem* Instance();
	void Update();
	void ShowCursor(bool is_show);
	void SetPosCursor(Point2d pos);
	void AddListener(IInputListener* p_listener);
	void RemoveListener(IInputListener* p_listener);
private:
	InputSystem();
	virtual ~InputSystem();
	void Notify(byte key, bool is_down);
	void KeyboardHandle();
	void MouseHandle();
protected:
	byte m_key_status[256];
	byte m_old_key_status[256];
	std::set<IInputListener*> m_listeners;
	Point2d m_cur_mouse_pos;
	Point2d m_old_mouse_pos;
	static InputSystem* sp_is;

};

