#include "pch.h"
#include "InputSystem.h"
#include "IInputListener.h"

InputSystem::InputSystem()
{
}

InputSystem::~InputSystem()
{
	m_listeners.clear();
}

void InputSystem::Notify(byte key, bool is_down)
{
	for (auto l : m_listeners)
	{
		if (is_down)
		{
			if (key == VK_LBUTTON)
				l->OnMouseLKeyDown(m_cur_mouse_pos);
			else if (key == VK_RBUTTON)
				l->OnMouseRKeyDown(m_cur_mouse_pos);
			else
				l->OnKeyDown(key);
		}
		else
		{
			if (key == VK_LBUTTON)
				l->OnMouseLKeyUp(m_cur_mouse_pos);
			else if (key == VK_RBUTTON)
				l->OnMouseRKeyUp(m_cur_mouse_pos);
			else
				l->OnKeyUp(key);
		}
	}
}

void InputSystem::KeyboardHandle()
{
	if (::GetKeyboardState(m_key_status))
	{
		int i = 0;
		for (byte k : m_key_status)
		{
			if (k & 0x80)
				Notify(i, true);
			else
			{
				if (m_old_key_status[i] != k)
					Notify(i, false);
			}
			++i;
		}
		memcpy(m_old_key_status, m_key_status, ARRAYSIZE(m_key_status));
	}
}

void InputSystem::MouseHandle()
{
	POINT pt;
	if (::GetCursorPos(&pt))
	{
		m_cur_mouse_pos.m_x = pt.x;
		m_cur_mouse_pos.m_y = pt.y;

		if (m_cur_mouse_pos != m_old_mouse_pos)
		{
			for (auto l : m_listeners)
			{
				l->OnMouseMove(m_cur_mouse_pos - m_old_mouse_pos);
			}
		}
	}
	m_old_mouse_pos = m_cur_mouse_pos;
}

InputSystem* InputSystem::Instance()
{
	static InputSystem inst;
	return &inst;
}

void InputSystem::Update()
{
	MouseHandle();
	KeyboardHandle();
}

void InputSystem::AddListener(IInputListener* p_listener)
{
	m_listeners.insert(p_listener);
}

void InputSystem::RemoveListener(IInputListener* p_listener)
{
	auto it = std::find(m_listeners.begin(), m_listeners.end(), p_listener);
	if (it != m_listeners.end())
		m_listeners.erase(it);
}

