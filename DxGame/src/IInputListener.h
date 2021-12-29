#pragma once

class IInputListener
{
public:
	IInputListener() {};
	virtual ~IInputListener() {};
	virtual void OnKeyDown(byte key) = 0;
	virtual void OnKeyUp(byte key) = 0;
};