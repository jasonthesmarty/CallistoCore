#pragma once

#include "pch.h"

typedef int CCint;
typedef long CClong;
typedef char CCchar;
typedef const char* CCwords;
typedef bool CCboolean;

enum CALLISTO_CORE_RESULT {
	CC_SUCCESS = 0,
	CC_FAILURE = 1
};

CALLISTO_CORE_RESULT CC_init_glfw();
CALLISTO_CORE_RESULT CC_create_window(GLFWwindow** window, CCint width, CCint height, CCwords title);
CCboolean CC_window_closing_status(GLFWwindow** window);
void CC_poll_events();
void CC_destroy_window(GLFWwindow** window);
void CC_terminate_glfw();