#include "input.h"

// InputHandler Class
InputHandler::InputHandler()
{
	for(int i; i < KEY_KEY_CODES_COUNT; ++i)
	{
		m_bKeyDown[i] = false;
	}
	m_bMouseLeftButtonDown = false;
	m_bMouseRightButtonDown = false;
}

bool InputHandler::OnEvent(const SEvent& event)
{
	static core::position2di f_pDragStartMousePosition;
	if(event.EventType == EET_KEY_INPUT_EVENT)
	{
		
		m_bKeyDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
	}
	else if(event.EventType == EET_MOUSE_INPUT_EVENT)
	{
		switch(event.MouseInput.Event)
		{
			case EMIE_LMOUSE_PRESSED_DOWN:
				m_bMouseLeftButtonDown = true;
				break;
			case EMIE_LMOUSE_LEFT_UP:
				m_bMouseLeftButtonDown = false;
				break;
			case EMIE_RMOUSE_PRESSED_DOWN:
				m_bMouseRightButtonDown = true;
				if(f_pDragStartMousePosition.X == 0 && f_pDragStartMousePosition.Y == 0)
				{
					f_pDragStartMousePosition.X = event.MouseInput.X;
					f_pDragStartMousePosition.Y = event.MouseInput.Y;
				}
				break;
			case EMIE_RMOUSE_LEFT_UP:
				m_bMouseRightButtonDown = false;

				m_iMouseDragX = f_pDragStartMousePosition.X - event.MouseInput.X;
				m_iMouseDragY = f_pDragStartMousePosition.Y - event.MouseInput.Y;
				f_pDragStartMousePosition.X = 0;
				f_pDragStartMousePosition.Y = 0;
				break;
			case EMIE_MOUSE_MOVED:
				m_pMousePosition.X = event.MouseInput.X;
				m_pMousePosition.Y = event.MouseInput.Y;
				break;
			case EMIE_MOUSE_WHEEL:
				m_fMouseWheelDelta = event.MouseInput.Wheel;
				break;
			default:
				break;
		}
	}
	return 0;
}

bool InputHandler::IsKeyDown(KEYBINDS keyCode)
{
	return m_bKeyDown[keyCode];
}

int InputHandler::MousePositionX()
{
	return m_pMousePosition.X;
}
int InputHandler::MousePositionY()
{
	return m_pMousePosition.Y;
}
float InputHandler::MouseWheelDelta()
{
	float tmp = m_fMouseWheelDelta;
	m_fMouseWheelDelta = 0;
	return tmp;
}
bool InputHandler::MouseLeftButtonDown()
{
	return m_bMouseLeftButtonDown;
}
bool InputHandler::MouseRightButtonDown()
{
	return m_bMouseRightButtonDown;
}
int InputHandler::MouseDragX()
{
	int tmp = m_iMouseDragX;
	m_iMouseDragX = 0;
	return tmp;
}
int InputHandler::MouseDragY()
{
	int tmp = m_iMouseDragY;
	m_iMouseDragY = 0;
	return tmp;
}
