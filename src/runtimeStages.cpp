#include <memory.h>
#include <glad/glad.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>
#include "runtimeStages.h"
#include "window.h"
#include "draw/draw.h"

static SDL_Window* sdlWindow;
static SDL_GLContext sdlGlContext;

static bool relativeMouseMode = false;

void Window::setRelativeMouseMode(bool enabled) {
	relativeMouseMode = enabled;
	SDL_SetRelativeMouseMode((SDL_bool)enabled);
}

bool Window::isRelativeMouseMode() {
	return relativeMouseMode;
}

static int eventWatcher(void* data, SDL_Event* event) {
	switch (event->type) {
		case SDL_QUIT:
			Window::shouldClose = true;
			break;

		case SDL_WINDOWEVENT:
			if (event->window.event == SDL_WINDOWEVENT_RESIZED) {
				int w, h;
				SDL_GL_GetDrawableSize(sdlWindow, &w, &h);
				glViewport(0, 0, w, h);
				Window::WIDTH = w;
				Window::HEIGHT = h;
			}
			break;

		case SDL_KEYDOWN:
			memcpy((void *)Window::keys, SDL_GetKeyboardState(nullptr), (int)Scancode::NUM_SCANCODES);

			if(Window::keyboardCallback != nullptr)
				Window::keyboardCallback();

			break;

		case SDL_KEYUP:
			memcpy((void*)Window::keys, SDL_GetKeyboardState(nullptr), (int)Scancode::NUM_SCANCODES);

			if (Window::keyboardCallback != nullptr)
				Window::keyboardCallback();

			break;

		case SDL_MOUSEMOTION:
			Window::mouseX = event->motion.x;
			Window::mouseY = event->motion.y;
			Window::mouseDX = event->motion.xrel;
			Window::mouseDY = event->motion.yrel;
			break;
	}

	return 0;
}

void RuntimeStages::init() {
	SDL_LogSetAllPriority(SDL_LOG_PRIORITY_WARN);

	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "UNABLE TO INITIALIZE SDL: %s", SDL_GetError());
		exit(1);
	}

	Window::WIDTH = 1280;
	Window::HEIGHT = 720;
	sdlWindow = SDL_CreateWindow("Planet game test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Window::WIDTH, Window::HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if (sdlWindow == nullptr) {
		SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "UNABLE TO CREATE WINDOW");
		exit(1);
	}

	SDL_AddEventWatch(eventWatcher, sdlWindow);
	Window::shouldClose = false;

	sdlGlContext = SDL_GL_CreateContext(sdlWindow);
	gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress);

	SDL_GL_SetSwapInterval(1);

	Draw::init();

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.IniFilename = nullptr;

	ImGui::StyleColorsDark();

	// Setup Platform/Renderer bindings
	ImGui_ImplSDL2_InitForOpenGL(sdlWindow, sdlGlContext);
	ImGui_ImplOpenGL3_Init("#version 330 core");
}

void RuntimeStages::run() {
	while (!Window::shouldClose) {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(sdlWindow);
		ImGui::NewFrame();

		Draw::update();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		Window::mouseDX = 0;
		Window::mouseDY = 0;

		SDL_GL_SwapWindow(sdlWindow);
		SDL_PollEvent(nullptr);
	}
}

void RuntimeStages::end() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_GL_DeleteContext(sdlGlContext);
	SDL_DestroyWindow(sdlWindow);
	SDL_Quit();
}