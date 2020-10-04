#pragma once

#include <scancode.h>

#define window_key(x) (Window::keys[(int)Scancode::x] == 1)

namespace Window {
	inline int WIDTH;
	inline int HEIGHT;
	inline bool shouldClose = false;

	inline int mouseX = 0;
	inline int mouseY = 0;
	inline int mouseDX = 0;
	inline int mouseDY = 0;

	inline int time = 0;
	inline int dTime = 0;

	void setRelativeMouseMode(bool enabled);
	bool isRelativeMouseMode();

	using keyboardCallbackFunc = void (*)();
	inline keyboardCallbackFunc keyboardCallback = nullptr;
	inline const char* keys = (char*)malloc((int)Scancode::NUM_SCANCODES);
}