#pragma once

#include <scancode.h>

#define window_key(x) (Window::keys[(int)Scancode::x] == 1)

namespace Window {
	inline int WIDTH;
	inline int HEIGHT;
	inline bool shouldClose;

	inline int mouseX;
	inline int mouseY;
	inline int mouseDX;
	inline int mouseDY;

	void setRelativeMouseMode(bool enabled);
	bool isRelativeMouseMode();

	using keyboardCallbackFunc = void (*)();
	inline keyboardCallbackFunc keyboardCallback = nullptr;
	inline const char* keys = (char*)malloc((int)Scancode::NUM_SCANCODES);
}