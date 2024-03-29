#include "Engine.h"
#include "Colors.h"

Engine * Engine::app = 0;

Engine::Engine(const char* pWindowName, const int Width, const int Height)
{
	assert(pWindowName);

	strcpy_s(info.title, 128, pWindowName);
	info.windowWidth = Width;
	info.windowHeight = Height;

	info.majorVersion = 4;
	info.minorVersion = 3;

	info.samples = 0;
	info.flags.all = 0;
	info.flags.cursor = 1;

#ifdef _DEBUG
	info.flags.debug = 1;
#endif
}

Engine::Engine()
{
}

Engine::~Engine()
{
}

//------------------------------------------------------------------
//	Game Loop
//------------------------------------------------------------------
void Engine::Run()
{
	this->privPreInitialize();

	Initialize(); // virtual

	this->privPreLoadContent();

	LoadContent(); // virtual

	while ((glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE) && (glfwWindowShouldClose(window) != GL_TRUE))
	{
		Update((float)glfwGetTime());

		// clear screen
		ClearBufferFunc();

		// render objects
		Draw();  // Virtual

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	UnLoadContent(); // virtual

	glfwDestroyWindow(window);
	glfwTerminate();
}

void Engine::privPreInitialize()
{
	app = this;

	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return;
	}
}

void Engine::privPreLoadContent()
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, info.majorVersion);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, info.minorVersion);

#ifndef _DEBUG
	if (info.flags.debug)
#endif /* _DEBUG */
	{
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	}
	if (info.flags.robust)
	{
		glfwWindowHint(GLFW_CONTEXT_ROBUSTNESS, GLFW_LOSE_CONTEXT_ON_RESET);
	}
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_SAMPLES, info.samples);
	glfwWindowHint(GLFW_STEREO, info.flags.stereo ? GL_TRUE : GL_FALSE);
	if (info.flags.fullscreen)
	{
		if (info.windowWidth == 0 || info.windowHeight == 0)
		{
			const GLFWvidmode *pMode;

			pMode = glfwGetVideoModes(*glfwGetMonitors(0), 0);

			info.windowWidth = pMode->width;
			info.windowHeight = pMode->height;
		}

		window = glfwCreateWindow(info.windowWidth, info.windowHeight, info.title, glfwGetPrimaryMonitor(), 0);

		glfwSwapInterval((int)info.flags.vsync);
	}
	else
	{
		window = glfwCreateWindow(info.windowWidth, info.windowHeight, info.title, info.flags.fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);
		if (!window)
		{
			fprintf(stderr, "Failed to open window\n");
			return;
		}
	}

	glfwMakeContextCurrent(window);

	glfwSetWindowSizeCallback(window, glfw_onResize);
	glfwSetKeyCallback(window, glfw_onKey);
	glfwSetMouseButtonCallback(window, glfw_onMouseButton);
	glfwSetCursorPosCallback(window, glfw_onMouseMove);
	glfwSetScrollCallback(window, glfw_onMouseWheel);
	if (!info.flags.cursor)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	}

	// info.flags.stereo = (glfwGetWindowParam(GLFW_STEREO) ? 1 : 0);

	gl3wInit();

#ifdef _DEBUG
	fprintf(stderr, "VENDOR: %s\n", (char *)glGetString(GL_VENDOR));
	fprintf(stderr, "VERSION: %s\n", (char *)glGetString(GL_VERSION));
	fprintf(stderr, "RENDERER: %s\n", (char *)glGetString(GL_RENDERER));
#endif

	if (info.flags.debug)
	{
		if (gl3wIsSupported(4, 3))
		{
			glDebugMessageCallback((GLDEBUGPROC)debug_callback, this);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		}
		else if (sb6IsExtensionSupported("GL_ARB_debug_output"))
		{
			glDebugMessageCallbackARB((GLDEBUGPROC)debug_callback, this);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
		}
	}

	//fix frame issue
	glfwSwapInterval(1);
	
	// setPosUV state
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
}

void Engine::Initialize()
{

}

void Engine::LoadContent()
{
}

void Engine::Draw()
{
}

void Engine::UnLoadContent()
{
}

void Engine::ClearBufferFunc()
{
	const GLfloat one = 1.0f;

	glViewport(0, 0, info.windowWidth, info.windowHeight);
	glClearBufferfv(GL_COLOR, 0, Colors_gl::glGray);
	glClearBufferfv(GL_DEPTH, 0, &one);
}

void APIENTRY Engine::debug_callback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	GLvoid* userParam)
{
	reinterpret_cast<Engine *>(userParam)->onDebugMessage(source, type, id, severity, length, message);
}

void Engine::setWindowTitle(const char * title)
{
	glfwSetWindowTitle(window, title);
}

void Engine::onResize(int w, int h)
{
	info.windowWidth = w;
	info.windowHeight = h;
}

void Engine::onKey(int key, int action)
{
	shutUp(key);
	shutUp(action);
}

int Engine::GetKey(int key)
{
	return glfwGetKey(window, key);
}

int Engine::GetMouseButton(int button)
{
	return glfwGetMouseButton(window, button);
}

void Engine::GetCursorPos(float &xPos, float &yPos)
{
	double xPosDouble;
	double yPosDouble;

	glfwGetCursorPos(window, &xPosDouble, &yPosDouble);

	xPos = (float)xPosDouble;
	yPos = (float)yPosDouble;
}

void Engine::onMouseButton(int button, int action)
{
	shutUp(button);
	shutUp(action);
}

void Engine::onMouseMove(int x, int y)
{
	shutUp(x);
	shutUp(y);
}

void Engine::onMouseWheel(int pos)
{
	shutUp(pos);
}

void Engine::onDebugMessage(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message)
{
	shutUp(length);
	shutUp(severity);
	shutUp(id);
	shutUp(type);
	shutUp(source);

#ifdef _WIN32
	OutputDebugStringA(message);
	OutputDebugStringA("\n");
#endif /* _WIN32 */
}

void Engine::getMousePosition(int& x, int& y)
{
	double dx, dy;
	glfwGetCursorPos(window, &dx, &dy);

	x = static_cast<int>(floor(dx));
	y = static_cast<int>(floor(dy));
}

void Engine::glfw_onResize(GLFWwindow* window, int w, int h)
{
	shutUp(window);

	app->onResize(w, h);
}

void Engine::glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	shutUp(mods);
	shutUp(scancode);
	shutUp(window);

	app->onKey(key, action);
}

void Engine::glfw_onMouseButton(GLFWwindow* window, int button, int action, int mods)
{
	shutUp(mods);
	shutUp(window);

	app->onMouseButton(button, action);
}

void Engine::glfw_onMouseMove(GLFWwindow* window, double x, double y)
{
	shutUp(window);

	app->onMouseMove(static_cast<int>(x), static_cast<int>(y));
}

void Engine::glfw_onMouseWheel(GLFWwindow* window, double xoffset, double yoffset)
{
	shutUp(window);
	shutUp(xoffset);

	app->onMouseWheel(static_cast<int>(yoffset));
}

void Engine::setVsync(bool enable)
{
	info.flags.vsync = enable ? 1u : 0u;
	glfwSwapInterval((int)info.flags.vsync);
}