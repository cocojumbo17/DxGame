#pragma once
#include "Point2d.h"

class IInputListener
{
public:
	IInputListener() {};
	virtual ~IInputListener() {};

	// Keyboard
	virtual void OnKeyDown(byte key) = 0;
	virtual void OnKeyUp(byte key) = 0;

	// Mouse
	virtual void OnMouseMove(const Point2d& pos) = 0;
	virtual void OnMouseLKeyDown(const Point2d& pos) = 0;
	virtual void OnMouseLKeyUp(const Point2d& pos) = 0;
	virtual void OnMouseRKeyDown(const Point2d& pos) = 0;
	virtual void OnMouseRKeyUp(const Point2d& pos) = 0;
};