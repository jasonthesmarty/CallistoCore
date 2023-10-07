#include "CallistoCore.h"

GLFWwindow* window;

int main() {
	CC_init_glfw();
	CC_create_window(&window, 1280, 720, "C++ Vulkan Application");

	while (!CC_window_closing_status(&window)) {
		CC_poll_events();
	}

	CC_destroy_window(&window);
	CC_terminate_glfw();
}