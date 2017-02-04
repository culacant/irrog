#ifndef INPUT_H
#define INPUT_H

#include <irrlicht/irrlicht.h>
#include "binds.h"

using namespace irr;

// InputHandler Class

class InputHandler : public IEventReceiver
{
private:
	bool m_bKeyDown[KEY_KEY_CODES_COUNT];

	core::position2di m_pMousePosition;
	float m_fMouseWheelDelta;
	bool m_bMouseLeftButtonDown;
	bool m_bMouseRightButtonDown;
	int m_iMouseDragX;
	int m_iMouseDragY;

public:
	InputHandler();

	bool OnEvent(const SEvent& event);
	bool IsKeyDown(KEYBINDS keyCode);

	int MousePositionX();
	int MousePositionY();

	float MouseWheelDelta();

	bool MouseLeftButtonDown();
	bool MouseRightButtonDown();

	int MouseDragX();
	int MouseDragY();

};

#endif
