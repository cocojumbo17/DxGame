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
			l->OnKeyDown(key);
		else
			l->OnKeyUp(key);
	}
}

InputSystem* InputSystem::Instance()
{
	static InputSystem inst;
	return &inst;
}

void InputSystem::Update()
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
	}
	memcpy(m_old_key_status, m_key_status, ARRAYSIZE(m_key_status));
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

