#include "pch.h"
#include "CallistoCore.h"

CALLISTO_CORE_RESULT CC_init_glfw()
{
	if (glfwInit()) {
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		return CC_SUCCESS;
	}
	return CC_FAILURE;
}

CALLISTO_CORE_RESULT CC_create_window(GLFWwindow** window, CCint width, CCint height, CCwords title)
{
	*window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if (*window == nullptr) {
		return CC_FAILURE;
	}
	return CC_SUCCESS;
}

CCboolean CC_window_closing_status(GLFWwindow** window)
{
	return glfwWindowShouldClose(*window);
}

void CC_poll_events() 
{
	glfwPollEvents();
}

void CC_destroy_window(GLFWwindow** window)
{
	glfwDestroyWindow(*window);
}

void CC_terminate_glfw()
{
	glfwTerminate();
}