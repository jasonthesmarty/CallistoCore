#pragma once

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

#include <algorithm>
#include <array>
#include <iostream>
#include <optional>
#include <set>
#include <string>
#include <vector>

#include "FMOD/fmod.hpp"
#include "glm/glm.hpp"
#include "shaderc/shaderc.hpp"
#include "stb/stb_image.h"
#include "ft2build.h"
#include FT_FREETYPE_H

/*
*	Note: [Twitch Studio] throws errors in validation layers. (just my experience)
*/

//<<----- Typedefs ------>//

// Added in 0.1
typedef int CCint;
typedef float CCfloat;
typedef double CCdouble;
typedef long CClong;

//Added in 0.2
typedef unsigned long CCulong;
typedef unsigned long long CCull;

// Added in 0.1
typedef char CCchar;
typedef const char* CCwords;
typedef bool CCboolean;
typedef uint16_t CCuint16;
typedef uint32_t CCuint32;
typedef uint64_t CCuint64;

//<<----- Typedefs ------>//

//<<----- Enums ------>//

// Added in 0.1
enum CALLISTO_CORE_RESULT {
	// Added in 0.1
	CC_PLACEHOLDER = 65535,
	
	CC_GLFW_INIT_SUCCESS = 1,
	CC_GLFW_INIT_FAILURE = 2,
	CC_GLFW_CREATE_WINDOW_SUCCESS = 3,
	CC_GLFW_CREATE_WINDOW_FAILURE = 4,
	
	CC_VK_VALIDATION_LAYERS_NOT_FOUND = 5,
	CC_VK_INSTANCE_CREATION_SUCCESS = 6,
	CC_VK_INSTANCE_CREATION_FAILURE = 7,
	CC_VK_VALIDATION_CREATION_FAILURE = 8,
	CC_VK_NO_PHYSICAL_DEVICE_FOUND = 9,
	CC_VK_PHYSICAL_DEVICE_WITH_NECESSARY_QUEUE_FAMILIES_FOUND = 10,
	CC_VK_PHYSICAL_DEVICE_WITH_NECESSARY_QUEUE_FAMILIES_NOT_FOUND = 11,
	CC_VK_LOGICAL_DEVICE_CREATION_SUCCESS = 12,
	CC_VK_LOGICAL_DEVICE_CREATION_FAILURE = 13,
	CC_VK_SURFACE_CREATION_SUCCESS = 14,
	CC_VK_SURFACE_CREATION_FAILURE = 15,
	CC_VK_SWAPCHAIN_CREATION_SUCCESS = 16,
	CC_VK_SWAPCHAIN_CREATION_FAILURE = 17,
	CC_VK_SWAPCHAIN_IMAGE_VIEWS_CREATION_SUCCESS = 18,
	CC_VK_SWAPCHAIN_IMAGE_VIEWS_CREATION_FAILURE = 19,
	CC_VK_SHADER_MODULE_COMPILATION_FAILURE = 20,
	CC_VK_SHADER_MODULE_CREATION_SUCCESS = 21,
	CC_VK_SHADER_MODULE_CREATION_FAILURE = 22,
	CC_VK_GRAPHICS_PIPELINE_LAYOUT_CREATION_SUCCESS = 23,
	CC_VK_GRAPHICS_PIPELINE_LAYOUT_CREATION_FAILURE = 24,
	CC_VK_RENDER_PASS_CREATION_SUCCESS = 25,
	CC_VK_RENDER_PASS_CREATION_FAILURE = 26,
	CC_VK_GRAPHICS_PIPELINE_CREATION_SUCCESS = 27,
	CC_VK_GRAPHICS_PIPELINE_CREATION_FAILURE = 28,
	CC_VK_FRAMEBUFFERS_CREATION_SUCCESS = 29,
	CC_VK_FRAMEBUFFERS_CREATION_FAILURE = 30,
	CC_VK_COMMAND_POOL_CREATION_SUCCESS = 31,
	CC_VK_COMMAND_POOL_CREATION_FAILURE = 32,
	CC_VK_COMMAND_BUFFER_CREATION_SUCCESS = 33,
	CC_VK_COMMAND_BUFFER_CREATION_FAILURE = 34,
	CC_VK_COMMAND_BUFFER_STARTED_SUCCESFULLY = 35,
	CC_VK_COMMAND_BUFFER_STARTED_FAILED = 36,
	CC_VK_RENDERPASS_STARTED_SUCCESFULLY = 37,
	CC_VK_RENDERPASS_STARTED_FAILED = 38,
	CC_VK_SEMAPHORE_CREATION_SUCCESS = 39,
	CC_VK_SEMAPHORE_CREATION_FAILURE = 40,
	CC_VK_FENCE_CREATION_SUCCESS = 41,
	CC_VK_FENCE_CREATION_FAILURE = 42,
	CC_VK_COMMAND_BUFFER_ENDED_SUCCESFULLY = 43,
	CC_VK_COMMAND_BUFFER_ENDED_FAILED = 44,
	CC_VK_QUEUE_SUBMITTED_SUCCESSFULLY = 45,
	CC_VK_QUEUE_SUBMITTED_FAILED = 46,
	CC_VK_QUEUE_PRESENTED_SUCCESSFULLY = 47,
	CC_VK_QUEUE_PRESENTED_FAILED = 48,
	CC_VK_BUFFER_CREATION_SUCCESS = 49,
	CC_VK_BUFFER_CREATION_FAILURE = 50,
	CC_VK_MEMORY_ALLOCATED_SUCCESS = 51,
	CC_VK_MEMORY_ALLOCATED_FAILURE = 52,
	CC_VK_DESCRIPTOR_SET_LAYOUT_CREATION_SUCCESS = 53,
	CC_VK_DESCRIPTOR_SET_LAYOUT_CREATION_FAILURE = 54,
	CC_VK_DESCRIPTOR_POOL_CREATION_SUCCESS = 55,
	CC_VK_DESCRIPTOR_POOL_CREATION_FAILURE = 56,
	CC_VK_DESCRIPTOR_SET_CREATION_SUCCESS = 57,
	CC_VK_DESCRIPTOR_SET_CREATION_FAILURE = 58,
	CC_IMAGE_DATA_LOADED_SUCCESSFULLY = 59,
	CC_IMAGE_DATA_LOADED_FAILED = 60,
	CC_VK_IMAGE_CREATION_SUCCESS = 61,
	CC_VK_IMAGE_CREATION_FAILURE = 62,
	CC_VK_IMAGE_SAMPLER_CREATION_SUCCESS = 63,
	CC_VK_IMAGE_SAMPLER_CREATION_FAILURE = 64,
	CC_VK_IMAGE_VIEW_CREATION_SUCCESS = 65,
	CC_VK_IMAGE_VIEW_CREATION_FAILURE = 66,

	// Added in 0.2
	CC_FMD_SOUND_SYSTEM_CREATION_SUCCESS = 472,
	CC_FMD_SOUND_SYSTEM_CREATION_FAILURE = 473,
	CC_FMD_SOUND_SYSTEM_INITIALIZATION_SUCCESS = 474,
	CC_FMD_SOUND_SYSTEM_INITIALIZATION_FAILURE = 475,
	CC_FMD_SOUND_CREATION_SUCCESS = 476,
	CC_FMD_SOUND_CREATION_FAILURE = 477,

	CC_FT_INITIALIZED_SUCCESSFULLY = 2048,
	CC_FT_INITIALIZED_UNSUCCESSFULLY = 2049,
	CC_FT_FACE_CREATION_SUCCESS = 2050,
	CT_FT_FACE_CREATION_FAILED = 2051,
};
// Added in 0.1
enum CALLISTO_CORE_KEYBOARD {
	CC_KEY_UNKNOWN = -1,
	CC_KEY_SPACE = 32,
	CC_KEY_APOSTROPHE = 39, 
	CC_KEY_COMMA = 44, 
	CC_KEY_MINUS = 45, 
	CC_KEY_PERIOD = 46, 
	CC_KEY_SLASH = 47, 
	CC_KEY_0 = 48,
	CC_KEY_1 = 49,
	CC_KEY_2 = 50,
	CC_KEY_3 = 51,
	CC_KEY_4 = 52,
	CC_KEY_5 = 53,
	CC_KEY_6 = 54,
	CC_KEY_7 = 55,
	CC_KEY_8 = 56,
	CC_KEY_9 = 57,
	CC_KEY_SEMICOLON = 59, 
	CC_KEY_EQUAL = 61, 
	CC_KEY_A = 65,
	CC_KEY_B = 66,
	CC_KEY_C = 67,
	CC_KEY_D = 68,
	CC_KEY_E = 69,
	CC_KEY_F = 70,
	CC_KEY_G = 71,
	CC_KEY_H = 72,
	CC_KEY_I = 73,
	CC_KEY_J = 74,
	CC_KEY_K = 75,
	CC_KEY_L = 76,
	CC_KEY_M = 77,
	CC_KEY_N = 78,
	CC_KEY_O = 79,
	CC_KEY_P = 80,
	CC_KEY_Q = 81,
	CC_KEY_R = 82,
	CC_KEY_S = 83,
	CC_KEY_T = 84,
	CC_KEY_U = 85,
	CC_KEY_V = 86,
	CC_KEY_W = 87,
	CC_KEY_X = 88,
	CC_KEY_Y = 89,
	CC_KEY_Z = 90,
	CC_KEY_LEFT_BRACKET = 91,
	CC_KEY_BACKSLASH = 92,
	CC_KEY_RIGHT_BRACKET = 93,
	CC_KEY_GRAVE_ACCENT = 96, 
	CC_KEY_WORLD_1 = 161, 
	CC_KEY_WORLD_2 = 162, 
	CC_KEY_ESCAPE = 256,
	CC_KEY_ENTER = 257,
	CC_KEY_TAB = 258,
	CC_KEY_BACKSPACE = 259,
	CC_KEY_INSERT = 260,
	CC_KEY_DELETE = 261,
	CC_KEY_RIGHT = 262,
	CC_KEY_LEFT = 263,
	CC_KEY_DOWN = 264,
	CC_KEY_UP = 265,
	CC_KEY_PAGE_UP = 266,
	CC_KEY_PAGE_DOWN = 267,
	CC_KEY_HOME = 268,
	CC_KEY_END = 269,
	CC_KEY_CAPS_LOCK = 280,
	CC_KEY_SCROLL_LOCK = 281,
	CC_KEY_NUM_LOCK = 282,
	CC_KEY_PRINT_SCREEN = 283,
	CC_KEY_PAUSE = 284,
	CC_KEY_F1 = 290,
	CC_KEY_F2 = 291,
	CC_KEY_F3 = 292,
	CC_KEY_F4 = 293,
	CC_KEY_F5 = 294,
	CC_KEY_F6 = 295,
	CC_KEY_F7 = 296,
	CC_KEY_F8 = 297,
	CC_KEY_F9 = 298,
	CC_KEY_F10 = 299,
	CC_KEY_F11 = 300,
	CC_KEY_F12 = 301,
	CC_KEY_F13 = 302,
	CC_KEY_F14 = 303,
	CC_KEY_F15 = 304,
	CC_KEY_F16 = 305,
	CC_KEY_F17 = 306,
	CC_KEY_F18 = 307,
	CC_KEY_F19 = 308,
	CC_KEY_F20 = 309,
	CC_KEY_F21 = 310,
	CC_KEY_F22 = 311,
	CC_KEY_F23 = 312,
	CC_KEY_F24 = 313,
	CC_KEY_F25 = 314,
	CC_KEY_KP_0 = 320,
	CC_KEY_KP_1 = 321,
	CC_KEY_KP_2 = 322,
	CC_KEY_KP_3 = 323,
	CC_KEY_KP_4 = 324,
	CC_KEY_KP_5 = 325,
	CC_KEY_KP_6 = 326,
	CC_KEY_KP_7 = 327,
	CC_KEY_KP_8 = 328,
	CC_KEY_KP_9 = 329,
	CC_KEY_KP_DECIMAL = 330,
	CC_KEY_KP_DIVIDE = 331,
	CC_KEY_KP_MULTIPLY = 332,
	CC_KEY_KP_SUBTRACT = 333,
	CC_KEY_KP_ADD = 334,
	CC_KEY_KP_ENTER = 335,
	CC_KEY_KP_EQUAL = 336,
	CC_KEY_LEFT_SHIFT = 340,
	CC_KEY_LEFT_CONTROL = 341,
	CC_KEY_LEFT_ALT = 342,
	CC_KEY_LEFT_SUPER = 343,
	CC_KEY_RIGHT_SHIFT = 344,
	CC_KEY_RIGHT_CONTROL = 345,
	CC_KEY_RIGHT_ALT = 346,
	CC_KEY_RIGHT_SUPER = 347,
	CC_KEY_MENU = 348,
	CC_KEY_LAST = CC_KEY_MENU
};
// Added in 0.1
enum CALLISTO_CORE_MOUSE {
	CC_MOUSE_BUTTON_1 = 0,
	CC_MOUSE_BUTTON_2 = 1,
	CC_MOUSE_BUTTON_3 = 2,
	CC_MOUSE_BUTTON_4 = 3,
	CC_MOUSE_BUTTON_5 = 4,
	CC_MOUSE_BUTTON_6 = 5,
	CC_MOUSE_BUTTON_7 = 6,
	CC_MOUSE_BUTTON_8 = 7,
	CC_MOUSE_BUTTON_LAST = CC_MOUSE_BUTTON_8,
	CC_MOUSE_BUTTON_LEFT = CC_MOUSE_BUTTON_1,
	CC_MOUSE_BUTTON_RIGHT = CC_MOUSE_BUTTON_2,
	CC_MOUSE_BUTTON_MIDDLE = CC_MOUSE_BUTTON_3
};

// Added in 0.2
enum CALLISTO_CORE_INFO {
	MAJOR_VERSION = 0,
	MINOR_VERSION = 2,
	PATCH_VERSION = 0,
};

//<<----- Enums ------>//

//<<----- Internal Function Declarations ------>//

// Added in 0.1
CCboolean p_CC_VK_isValidationLayerSupported(const std::vector<CCwords>& validationLayers);

// Added in 0.1
VKAPI_ATTR VkBool32 VKAPI_CALL p_CC_VK_debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);

// Added in 0.1
CCboolean p_CC_VK_are_device_extensions_supported(VkPhysicalDevice physicalDevice, const std::vector<CCwords>& deviceExtensions);

// Added in 0.1
CCboolean p_CC_VK_does_device_have_required_families(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, const std::vector<CCwords>& deviceExtensions);

// Added in 0.1
VkSurfaceFormatKHR p_CC_VK_choose_swapchain_format(const std::vector<VkSurfaceFormatKHR>& formats, VkFormat colorFormat, VkColorSpaceKHR colorSpace);

// Added in 0.1
VkPresentModeKHR p_CC_VK_choose_swapchain_present_mode(const std::vector<VkPresentModeKHR>& presentModes, VkPresentModeKHR wantedPresentMode, VkPresentModeKHR defaultPresentMode);

// Added in 0.1
VkExtent2D p_CC_VK_choose_swapchain_extent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window);

// Added in 0.1
CCuint32 p_CC_VK_find_mem_type_index(VkPhysicalDevice* physicalDevice, CCuint32 typeFilter, VkMemoryPropertyFlags properties);

//<<----- Internal Function Declarations ------>//

//<<----- Public Function Declarations ------>//

//<<----- Util Functions ------>//

// Added in 0.2
void CC_get_version();

#ifdef CC_USEFUL

// Added in 0.2
template <typename T>
inline void CC_println(T type) {
	std::cout << type << '\n';
}

// Added in 0.2
template <typename T>
inline void CC_print(T type) {
	std::cout << type;
}

// Added in 0.2
template <typename T>
inline T CC_input() {
	T in{};
	std::cin >> in;
	return in;
}

// Added in 0.2
template <typename T>
inline T CC_input(CCwords msg) {
	T in{};
	CC_print(msg);
	std::cin >> in;
	return in;
}

#endif

//<<----- Util Functions ------>//

//<<----- GLFW ------>//

// Added in 0.1
CALLISTO_CORE_RESULT CC_init_glfw();

// Added in 0.1
void CC_set_window_attrib_init(CCint hint, CCint value);

// Added in 0.1
CALLISTO_CORE_RESULT CC_create_window(GLFWwindow** window, CCint width, CCint height, CCwords title);

// Added in 0.1
void CC_set_window_attrib_runtime(GLFWwindow** window, CCint attrib, CCint value);

// Added in 0.1
CCboolean CC_window_closing_status(GLFWwindow** window);

// Added in 0.1
void CC_poll_events();

// Added in 0.1
void CC_framebuffer_size_change_callback(GLFWwindow** window, GLFWframebuffersizefun func);

// Added in 0.1
void CC_set_keyboard_callback(GLFWwindow** window, GLFWkeyfun func);

// Added in 0.1
void CC_set_mouse_callback(GLFWwindow** window, GLFWmousebuttonfun func);

// Added in 0.1
void CC_set_mouse_pos_callback(GLFWwindow** window, GLFWcursorposfun func);

// Added in 0.1
void CC_set_scroll_wheel_callback(GLFWwindow** window, GLFWscrollfun func);

// Added in 0.1
CCboolean CC_get_keyboard_input(CCint key, CCboolean* keys);

// Added in 0.1
CCboolean CC_get_mouse_input(CCint button, CCboolean* buttons);

// Added in 0.1
void CC_get_window_size(GLFWwindow** window, CCint* width, CCint* height);

// Added in 0.1
void CC_clock(CCdouble* sec);

// Added in 0.1
void CC_framerate(CCdouble* prevTime, CCint* frames, CCdouble* framerate, CCdouble* frametimes);

// Added in 0.1
void CC_set_window_title(GLFWwindow** window, CCwords title);

// Added in 0.1
void CC_set_window_size(GLFWwindow** window, CCint width, CCint height);

// Added in 0.1
void CC_set_window_position(GLFWwindow** window, CCint xPos, CCint yPos);

// Added in 0.1
void CC_set_window_opacity(GLFWwindow** window, CCint opacity);

// Added in 0.1
void CC_set_window_icon(GLFWwindow** window, CCwords filename);

// Added in 0.1
void CC_destroy_window(GLFWwindow** window);

// Added in 0.1
void CC_terminate_glfw();

//<<----- GLFW ------>//

//<<----- FMOD ------>//

// Added in 0.2
CALLISTO_CORE_RESULT CC_FMD_create_sound_system(FMOD::System** system);

// Added in 0.2
CALLISTO_CORE_RESULT CC_FMD_init_sound_system(FMOD::System* system, CCint maxChannels, FMOD_INITFLAGS flags);

// Added in 0.2
CALLISTO_CORE_RESULT CC_FMD_create_sound(FMOD::System* system, FMOD::Sound** sound, CCwords soundFile, FMOD_MODE mode);

// Added in 0.2
void CC_FMD_set_position(FMOD::Channel* channel, const FMOD_VECTOR* position, const FMOD_VECTOR* listenerOrientation);

// Added in 0.2
void CC_FMD_get_position(FMOD::Channel* channel, FMOD_VECTOR* position, FMOD_VECTOR* listenerOrientation);

// Added in 0.2
void CC_FMD_set_pan(FMOD::Channel* channel, CCfloat pan);

// Added in 0.2
void CC_FMD_set_pitch(FMOD::Channel* channel, CCfloat pitch);

// Added in 0.2
void CC_FMD_get_pitch(FMOD::Channel* channel, CCfloat* pitch);

// Added in 0.2
void CC_FMD_set_volume(FMOD::Channel* channel, CCfloat volume);

// Added in 0.2
void CC_FMD_get_volume(FMOD::Channel* channel, CCfloat* volume);

// Added in 0.2
void CC_FMD_set_priority(FMOD::Channel* channel, CCint priority);

// Added in 0.2
void CC_FMD_get_prioirty(FMOD::Channel* channel, CCint* priority);

// Added in 0.2
void CC_FMD_set_paused(FMOD::Channel* channel, CCboolean paused);

// Added in 0.2
void CC_FMD_get_paused(FMOD::Channel* channel, CCboolean* paused);

// Added in 0.2
void CC_FMD_set_mute(FMOD::Channel* channel, CCboolean mute);

// Added in 0.2
void CC_FMD_get_mute(FMOD::Channel* channel, CCboolean* mute);

// Added in 0.2
void CC_FMD_set_mode(FMOD::Channel* channel, FMOD_MODE mode);

// Added in 0.2
void CC_FMD_get_mode(FMOD::Channel* channel, FMOD_MODE* mode);

// Added in 0.2
void CC_FMD_set_timestamp(FMOD::Channel* channel, CCuint32 time, FMOD_TIMEUNIT timeMeasurement);

// Added in 0.2
void CC_FMD_get_timestamp(FMOD::Channel* channel, CCuint32* time, FMOD_TIMEUNIT timeMeasurement);

// Added in 0.2
void CC_FMD_play_sound(FMOD::System* system, FMOD::Sound* sound, FMOD::ChannelGroup* channelGroup, FMOD::Channel** channel, CCboolean paused);

// Added in 0.2
void CC_FMD_destroy_sound(FMOD::Sound* sound);

// Added in 0.2
void CC_FMD_destroy_sound_system(FMOD::System* system);

//<<----- FMOD ------>//

//<<----- FreeType ----->//

// Added in 0.2
CALLISTO_CORE_RESULT CC_FT_init(FT_Library* ft);

// Added in 0.2
CALLISTO_CORE_RESULT CC_FT_create_face(FT_Library* ft, FT_Face* face, CCwords fontFile);

// Added in 0.2
void CC_FT_set_pixel_size(FT_Face* face, CCuint32 fontWidthPX, CCuint32 fontHeightPX);

// Added in 0.2
void CC_FT_load_char(FT_Face* face, CCulong charCode, CCint flags);

// Added in 0.2
void CC_FT_destroy_face(FT_Face* face);

// Added in 0.2
void CC_FT_un_init(FT_Library* ft);

//<<----- FreeType ----->//

//<<----- Vulkan ------>//

// Added in 0.1
void CC_VK_create_app_info(VkApplicationInfo* appInfo, CCwords appName, CCwords engineName, std::array<CCint, 3> appVersion, std::array<CCint, 3> engineVersion, CCuint32 vulkanApiVersion);

// Added in 0.1
CALLISTO_CORE_RESULT CC_VK_create_instance(VkInstance* instance, VkApplicationInfo* appInfo, VkDebugUtilsMessengerEXT* debugMessenger, const std::vector<CCwords>& validationLayers, CCboolean validationLayerToggle);

// Added in 0.1
void CC_VK_destroy_instance(VkInstance* instance, VkDebugUtilsMessengerEXT* debugMessenger, CCboolean validationLayerToggle);


// Added in 0.1
CALLISTO_CORE_RESULT CC_VK_locate_physical_device(VkInstance* instance, VkPhysicalDevice* physicalDevice, VkSurfaceKHR* surface, const std::vector<CCwords>& deviceExtensions);

// Added in 0.1
CALLISTO_CORE_RESULT CC_VK_create_logical_device(VkInstance* instance, VkPhysicalDevice* physicalDevice, VkDevice* logicalDevice, VkSurfaceKHR* surface, VkQueue* graphicsQueue, VkQueue* computeQueue, VkQueue* transferQueue, VkQueue* presentQueue, const std::vector<CCwords>& validationLayers, const std::vector<CCwords>& deviceExtensions, CCboolean validationLayerToggle);

// Added in 0.1
void CC_VK_destroy_logical_device(VkDevice* logicalDevice);


// Added in 0.1
CALLISTO_CORE_RESULT CC_VK_create_surface(VkInstance* instance, GLFWwindow** window, VkSurfaceKHR* surface);

// Added in 0.1
void CC_VK_destroy_surface(VkInstance* instance, VkSurfaceKHR* surface);


// Added in 0.1
CALLISTO_CORE_RESULT CC_VK_create_swapchain(VkPhysicalDevice* physicalDevice, VkDevice* logicalDevice, VkSurfaceKHR* surface, GLFWwindow** window, VkSwapchainKHR* swapchain, std::vector<VkImage>& swapchainImages, VkFormat* swapchainImageFormat, VkExtent2D* swapchainExtent, VkFormat colorFormat, VkColorSpaceKHR colorSpace, VkPresentModeKHR wantedPresentMode, VkPresentModeKHR defaultPresentMode, VkImageUsageFlagBits imageUsage, VkCompositeAlphaFlagBitsKHR compositeAlpha);

// Added in 0.1
void CC_VK_destroy_swapchain(VkDevice* logicalDevice, VkSwapchainKHR* swapchain);


// Added in 0.1
CALLISTO_CORE_RESULT CC_VK_create_image_views(VkDevice* logicalDevice, std::vector<VkImageView>& swapchainImageViews, std::vector<VkImage>& swapchainImages, VkFormat* swapchainImageFormat, VkImageViewType imageViewType, VkComponentSwizzle redSwizzle, VkComponentSwizzle greenSwizzle, VkComponentSwizzle blueSwizzle, VkComponentSwizzle alphaSwizzle, VkImageAspectFlagBits aspectMask);

// Added in 0.1
void CC_VK_destroy_image_views(VkDevice* logicalDevice, std::vector<VkImageView>& swapchainImageViews);


// Added in 0.1
void CC_VK_color_attachment_desc(VkAttachmentDescription* colorAttachmentDesc, VkFormat* format, VkSampleCountFlagBits samples, VkAttachmentLoadOp loadOp, VkAttachmentStoreOp storeOp, VkAttachmentLoadOp stencilLoadOp, VkAttachmentStoreOp stencilStoreOp, VkImageLayout imageLayout, VkImageLayout finalLayout);

// Added in 0.1
void CC_VK_color_attachment_ref(VkAttachmentReference* colorAttachmentRef, CCuint32 attachment, VkImageLayout layout);

// Added in 0.1
void CC_VK_subpass_desc(VkSubpassDescription* subpassDesc, VkPipelineBindPoint pipelineBindPoint, CCuint32 colorAttachmentCount, const VkAttachmentReference* pColorAttachments);

// Added in 0.1
CALLISTO_CORE_RESULT CC_VK_create_renderpass(VkRenderPass* renderpass, VkDevice* logicalDevice, CCuint32 attachmentCount, const VkAttachmentDescription* colorAttachmentDesc, CCuint32 subpassCount, const VkSubpassDescription* subpassDesc);

// Added in 0.1
void CC_VK_destroy_renderpass(VkDevice* logicalDevice, VkRenderPass* renderpass);


// Added in 0.1
void CC_VK_descriptor_set_layout_binding(VkDescriptorSetLayoutBinding* descSetLayBind, CCuint32 binding, CCuint32 descriptorCount, VkDescriptorType descriptorType, VkSampler* immutableSamplers, VkShaderStageFlags stageFlags);

// Added in 0.1
CALLISTO_CORE_RESULT CC_VK_create_descriptor_set_layout(VkDevice* logicalDevice, VkDescriptorSetLayout* descSetLayout, CCuint32 bindingCount, const VkDescriptorSetLayoutBinding* bindings);

// Added in 0.1
void CC_VK_create_descriptor_pool_size(VkDescriptorPoolSize* descPoolSize, VkDescriptorType type, CCuint32 descriptorCount);

// Added in 0.1
CALLISTO_CORE_RESULT CC_VK_create_descriptor_pool(VkDevice* logicalDevice, VkDescriptorPool* descPool, CCuint32 poolSizeCount, const VkDescriptorPoolSize* poolSizes, CCuint32 maxSets);

// Added in 0.1
CALLISTO_CORE_RESULT CC_VK_create_descriptor_sets(VkDevice* logicalDevice, VkDescriptorSet* descSet, VkDescriptorPool* descPool, VkDescriptorSetLayout* descSetLayout);


// Added in 0.1
void CC_VK_get_physical_device_properties(VkPhysicalDeviceProperties* physDevProp, VkPhysicalDevice* physicalDevice);


// Added in 0.1
void CC_VK_descriptor_buffer_info(VkDescriptorBufferInfo* bufferInfo, VkBuffer buffer, VkDeviceSize offset, VkDeviceSize range);

// Added in 0.1
void CC_VK_descriptor_image_info(VkDescriptorImageInfo* bufferInfo, VkImageLayout imageLayout, VkImageView imageView, VkSampler sampler);


// Added in 0.1
void CC_VK_write_descriptor_sets_buffer(VkDescriptorSet* descSet, VkWriteDescriptorSet* descWrite, CCuint32 dstBinding, CCuint32 dstArrayElement, VkDescriptorType descriptorType, CCuint32 descriptorCount, const VkDescriptorBufferInfo* bufferInfo);

// Added in 0.1
void CC_VK_write_descriptor_sets_image(VkDescriptorSet* descSet, VkWriteDescriptorSet* descWrite, CCuint32 dstBinding, CCuint32 dstArrayElement, VkDescriptorType descriptorType, CCuint32 descriptorCount, const VkDescriptorImageInfo* imageInfo);


// Added in 0.1
void CC_VK_update_descriptor_sets(VkDevice* logicalDevice, CCuint32 descWriteCount, const VkWriteDescriptorSet* descWrite, CCuint32 descCopyCount, const VkCopyDescriptorSet* descCopy);


// Added in 0.1
void CC_VK_destroy_descriptor_pool(VkDevice* logicalDevice, VkDescriptorPool* descPool);

// Added in 0.1
void CC_VK_destroy_descriptor_set_layout(VkDevice* logicalDevice, VkDescriptorSetLayout* descSetLayout);


// Added in 0.1
CALLISTO_CORE_RESULT CC_VK_create_shader_module(VkDevice* logicalDevice, VkShaderModule* shaderProgram, CCwords shaderCode, CCwords entryPointName, shaderc_shader_kind shaderType);

// Added in 0.1
void CC_VK_create_graphics_pipeline_shader_stage(VkPipelineShaderStageCreateInfo* shaderStageCreateInfo, VkShaderStageFlagBits shaderStage, VkShaderModule* shaderModule, CCwords entryPoint);


// Added in 0.1
void CC_VK_vertex_input_binding_desc(VkVertexInputBindingDescription* vertexInputBindDesc, CCuint32 binding, CCuint32 stride, VkVertexInputRate inputRate);

// Added in 0.1
void CC_VK_vertex_input_attrib_desc(VkVertexInputAttributeDescription* vertexInputAttribDesc, CCuint32 binding, CCuint32 location, VkFormat format, CCuint32 offset);


// Added in 0.1
void CC_VK_pipe_vertex_input_state(VkPipelineVertexInputStateCreateInfo* vertexInputStateInfo, CCuint32 vertexInputBindDescCount, const VkVertexInputBindingDescription* vertexInputBindDesc, CCuint32 vertexInputAttribDescCount, const VkVertexInputAttributeDescription* vertexInputAttribDesc);

// Added in 0.1
void CC_VK_pipe_input_assembly_state(VkPipelineInputAssemblyStateCreateInfo* inputAssemblyStateInfo, VkPrimitiveTopology topology, VkBool32 primitiveRestartEnable);

// Added in 0.1
void CC_VK_pipe_viewport_state(VkPipelineViewportStateCreateInfo* viewportStateInfo, CCuint32 viewportCount, CCuint32 scissorCount);

// Added in 0.1
void CC_VK_pipe_rasterization_state(VkPipelineRasterizationStateCreateInfo* rasterizerStateInfo, VkBool32 depthClampEnable, VkBool32 rasterizerDiscardEnable, VkPolygonMode polygonMode, CCfloat lineWidth, VkCullModeFlags cullMode, VkFrontFace frontFace, VkBool32 depthBiasEnable);

// Added in 0.1
void CC_VK_pipe_multisample_state(VkPipelineMultisampleStateCreateInfo* multisamplingStateInfo, VkBool32 sampleShadingEnable, VkSampleCountFlagBits rasterizationSamples);

// Added in 0.1
void CC_VK_pipe_colorblend_attachment_state(VkPipelineColorBlendAttachmentState* colorblendAttachmentState, VkColorComponentFlags colorWriteMask, VkBool32 blendEnable, VkBlendFactor srcColorBlendFactor, VkBlendFactor dstColorBlendFactor, VkBlendOp colorBlendOp, VkBlendFactor srcAlphaBlendFactor, VkBlendFactor dstAlphaBlendFactor, VkBlendOp alphaBlendOp);

// Added in 0.1
void CC_VK_pipe_colorblend_state(VkPipelineColorBlendStateCreateInfo* colorblendStateInfo, VkPipelineColorBlendAttachmentState* colorBlendAttachmentState, VkBool32 logicOpEnable, VkLogicOp logicOp, std::array<CCfloat, 4> blendConstants);

// Added in 0.1
void CC_VK_pipe_colorblend_state(VkPipelineColorBlendStateCreateInfo* colorblendStateInfo, CCuint32 colorBlendAttachmentStatesCount, const VkPipelineColorBlendAttachmentState* colorBlendAttachmentStates, VkBool32 logicOpEnable, VkLogicOp logicOp, std::array<CCfloat, 4> blendConstants);

// Added in 0.1
void CC_VK_pipe_dynamic_state(VkPipelineDynamicStateCreateInfo* dynamicStateInfo, CCuint32 dynamicStatesCount, const VkDynamicState* dynamicStates);


// Added in 0.1
CALLISTO_CORE_RESULT CC_VK_create_graphics_pipeline_layout(VkDevice* logicalDevice, VkPipelineLayout* pipelineLayout, CCuint32 descSetLayoutsSize, const VkDescriptorSetLayout* descSetLayouts, CCuint32 pushConstantRangeCount, const VkPushConstantRange* pPushConstantRanges);

// Added in 0.1
CALLISTO_CORE_RESULT CC_VK_create_graphics_pipeline(VkDevice* logicalDevice, VkRenderPass* renderpass, VkPipeline* graphicsPipeline, CCuint32 shaderStagesCount, const VkPipelineShaderStageCreateInfo* shaderStages, VkPipelineVertexInputStateCreateInfo* vertexInputStateInfo, VkPipelineInputAssemblyStateCreateInfo* inputAssemblyStateInfo, VkPipelineViewportStateCreateInfo* viewportStateInfo, VkPipelineRasterizationStateCreateInfo* rasterizerStateInfo, VkPipelineMultisampleStateCreateInfo* multisamplingStateInfo, VkPipelineColorBlendStateCreateInfo* colorblendStateInfo, VkPipelineDynamicStateCreateInfo* dynamicStateInfo, VkPipelineLayout* pipelineLayout);


// Added in 0.1
void CC_VK_destroy_shader_module(VkDevice* logicalDevice, VkShaderModule* shaderModule);

// Added in 0.1
void CC_VK_destroy_graphics_pipeline_layout(VkDevice* logicalDevice, VkPipelineLayout* pipelineLayout);

// Added in 0.1
void CC_VK_destroy_graphics_pipeline(VkDevice* logicalDevice, VkPipeline* graphicsPipeline);


// Added in 0.1
CALLISTO_CORE_RESULT CC_VK_create_framebuffers(VkDevice* logicalDevice, std::vector<VkFramebuffer>& swapchainFramebuffers, const std::vector<VkImageView>& swapchainImageViews, VkRenderPass* renderpass, VkExtent2D* swapchainExtent);

// Added in 0.1
void CC_VK_destroy_framebuffers(VkDevice* logicalDevice, std::vector<VkFramebuffer>& swapchainFramebuffers);


// Added in 0.1
CALLISTO_CORE_RESULT CC_VK_create_command_pool(VkDevice* logicalDevice, VkCommandPool* commandPool, VkPhysicalDevice* physicalDevice, VkSurfaceKHR* surface, VkCommandPoolCreateFlags flags);

// Added in 0.1
CALLISTO_CORE_RESULT CC_VK_create_command_buffers(VkDevice* logicalDevice, VkCommandPool* commandPool, VkCommandBuffer* commandBuffer, VkCommandBufferLevel level);

// Added in 0.1
void CC_VK_destroy_command_pool(VkDevice* logicalDevice, VkCommandPool* commandPool);


// Added in 0.1
CALLISTO_CORE_RESULT CC_VK_create_semaphore(VkDevice* logicalDevice, VkSemaphore* semaphore, VkSemaphoreCreateFlags flags);

// Added in 0.1
CALLISTO_CORE_RESULT CC_VK_create_fence(VkDevice* logicalDevice, VkFence* fence, VkFenceCreateFlags flags);

// Added in 0.1
void CC_VK_wait_for_fences(VkDevice* logicalDevice, CCuint32 fenceCount, const VkFence* fences, VkBool32 waitForAll, CCuint64 timeout);

// Added in 0.1
void CC_VK_reset_fences(VkDevice* logicalDevice, CCuint32 fenceCount, const VkFence* fences);

// Added in 0.1
void CC_VK_destroy_semaphore(VkDevice * logicalDevice, VkSemaphore * semaphore);

// Added in 0.1
void CC_VK_destroy_fence(VkDevice* logicalDevice, VkFence* fence);


// Added in 0.1
CALLISTO_CORE_RESULT CC_VK_create_buffer(VkDevice* logicalDevice, VkBuffer* buffer, VkBufferCreateInfo* bufInfo, VkDeviceSize size, VkBufferUsageFlags usage, VkSharingMode sharingMode);

// Added in 0.1
void CC_VK_get_buffer_memory_requirements(VkDevice* logicalDevice, VkBuffer* buffer, VkMemoryRequirements* memRequirements);

// Added in 0.1
void CC_VK_get_image_memory_requirements(VkDevice* logicalDevice, VkImage* image, VkMemoryRequirements* memRequirements);

// Added in 0.1
CALLISTO_CORE_RESULT CC_VK_alloc_memory(VkDevice* logicalDevice, VkPhysicalDevice* physicalDevice, VkDeviceMemory* devMem, VkMemoryRequirements* memReq, VkMemoryPropertyFlags flags);

// Added in 0.1
void CC_VK_bind_buffer_memory(VkDevice* logicalDevice, VkBuffer* buffer, VkDeviceMemory* devMem, CCuint32 memOffset);

// Added in 0.1
void CC_VK_bind_image_memory(VkDevice* logicalDevice, VkImage* image, VkDeviceMemory* devMem, CCuint32 memOffset);


// Added in 0.1
template <typename T>
void CC_VK_map_cpy_unmap(VkDevice* logicalDevice, T* src, VkDeviceMemory* devMem, VkBufferCreateInfo* bufInfo);
#include "CallistoCore.ipp"


// Added in 0.1
CALLISTO_CORE_RESULT CC_VK_create_image_sampler(VkDevice* logicalDevice, VkSampler* sampler, VkFilter magFilter, VkFilter minFilter, VkSamplerAddressMode addressModeU, VkSamplerAddressMode addressModeV, VkSamplerAddressMode addressModeW, VkBool32 anisotropyEnable, VkPhysicalDeviceProperties  physDevProp, VkBorderColor borderColor, VkBool32 unnormalizedCoordinates, VkBool32 compareEnable, VkCompareOp compareOp, VkSamplerMipmapMode mipmapMode);

// Added in 0.1
CALLISTO_CORE_RESULT CC_VK_create_image_view(VkDevice* logicalDevice, VkImageView* imageView, VkImage* image, VkImageViewType viewType, VkFormat format, VkImageAspectFlags aspectMap, CCuint32 baseMipLevel, CCuint32 levelCount, CCuint32 baseArrayLayer, CCuint32 layerCount);


// Added in 0.1
void CC_VK_destroy_image(VkDevice* logicalDevice, VkImage* image);

// Added in 0.1
void CC_VK_destroy_image_sampler(VkDevice* logicalDevice, VkSampler* sampler);

// Added in 0.1
void CC_VK_destroy_image_view(VkDevice* logicalDevice, VkImageView* imageView);

// Added in 0.1
void CC_VK_destroy_buffer(VkDevice* logicalDevice, VkBuffer* buffer);

// Added in 0.1
void CC_VK_destroy_device_mem(VkDevice* logicalDevice, VkDeviceMemory* devMem);


// Added in 0.1
void CC_VK_map_mem(VkDevice* logicalDevice, VkDeviceMemory* devMem, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void** data);

// Added in 0.1
void CC_VK_unmap_mem(VkDevice* logicalDevice, VkDeviceMemory* devMem);


// Added in 0.1
CALLISTO_CORE_RESULT CC_load_image_data(stbi_uc** imageData, CCwords filename, CCint* imageWidth, CCint* imageHeight, CCint* imageChannels, CCint colorFormat);

// Added in 0.1
CALLISTO_CORE_RESULT CC_VK_create_image(VkDevice* logicalDevice, VkImage* image, VkImageType imageType, CCuint32 width, CCuint32 height, CCuint32 depth, CCuint32 mipLevels, CCuint32 arrayLayers, VkFormat format, VkImageTiling tiling, VkImageLayout initialLayout, VkImageUsageFlags usage, VkSampleCountFlagBits samples, VkSharingMode sharingMode);

// Added in 0.1
void CC_VK_copy_buffer_to_image(VkCommandBuffer* commandBuffer, VkBuffer* buffer, VkImage* image, VkDeviceSize bufferOffset, CCuint32 bufferRowLength, CCuint32 bufferImageHeight, VkImageAspectFlags aspectMask, CCuint32 mipLevel, CCuint32 baseArrayLayer, CCuint32 layerCount, CCint imageOffsetX, CCint imageOffsetY, CCint imageOffsetZ, CCuint32 width, CCuint32 height, CCuint32 depth);

// Added in 0.1
void CC_VK_pipeline_barrier(VkCommandBuffer* commandBuffer, VkImageLayout oldLayout, VkImageLayout newLayout, CCuint32 srcQueueFamilyIndex, CCuint32 dstQueueFamilyIndex, VkImage* image, VkImageAspectFlags aspectMask, CCuint32 baseMipLevel, CCuint32 levelCount, CCuint32 baseArrayLayer, CCuint32 layerCount, VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask, VkPipelineStageFlags sourceStage, VkPipelineStageFlags destinationStage);

// Added in 0.1
void CC_free_image_data(stbi_uc** imageData);


// Added in 0.1
CALLISTO_CORE_RESULT CC_VK_begin_command_buffers(VkCommandBuffer* commandBuffer, VkCommandBufferUsageFlags flags);

// Added in 0.1
void CC_VK_get_image_index_in_framebuffers(VkDevice* logicalDevice, VkSwapchainKHR* swapchain, CCuint64 timeout, VkSemaphore* semaphore, VkFence* fence, CCuint32* imageIndex);

// Added in 0.1
void CC_VK_begin_renderpass(VkCommandBuffer* commandBuffer, VkRenderPass* renderpass, const std::vector<VkFramebuffer>& swapchainFramebuffers, CCuint32 imageIndex, VkExtent2D* swapchainExtent);


// Added in 0.1
void CC_VK_bind_pipeline(VkCommandBuffer* commandBuffer, VkPipelineBindPoint bindPoint, VkPipeline* pipeline);


// Added in 0.1
void CC_VK_viewport(VkCommandBuffer* commandBuffer, CCfloat x, CCfloat y, CCfloat width, CCfloat height, CCfloat minDepth, CCfloat maxDepth);

// Added in 0.1
void CC_VK_scissor(VkCommandBuffer* commandBuffer, CCint topLeftX, CCint topLeftY, VkExtent2D* swapchainExtent);


// Added in 0.1
void CC_VK_bind_vertex_buffers(VkCommandBuffer* commandBuffer, CCuint32 firstBinding, CCuint32 bindingCount, const VkBuffer* buffer, const VkDeviceSize* offsets);

// Added in 0.1
void CC_VK_bind_index_buffers(VkCommandBuffer* commandBuffer, VkBuffer* indexBuffers, VkDeviceSize offset, VkIndexType indexType);

// Added in 0.1
void CC_VK_bind_descriptor_sets(VkCommandBuffer* commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout pipelineLayout, CCuint32 firstSet, CCuint32 descSetsCount, const VkDescriptorSet* descSets, CCuint32 offsetCount, const CCuint32* offsets);


// Added in 0.1
void CC_VK_draw(VkCommandBuffer* commandBuffer, CCuint32 vertexCount, CCuint32 instanceCount, CCuint32 firstVertex, CCuint32 firstInstance);

// Added in 0.1
void CC_VK_indexed_draw(VkCommandBuffer* commandBuffer, CCuint32 indexCount, CCuint32 instanceCount, CCuint32 firstIndex, CCuint32 vertexOffset, CCuint32 firstInstance);


// Added in 0.1
void CC_VK_end_renderpass(VkCommandBuffer* commandBuffer);

// Added in 0.1
CALLISTO_CORE_RESULT CC_VK_end_command_buffers(VkCommandBuffer* commandBuffer);

// Added in 0.1
void CC_VK_reset_command_buffers(VkCommandBuffer* commandBuffer, VkCommandBufferResetFlags flags);


// Added in 0.1
CALLISTO_CORE_RESULT CC_VK_submit_queue(VkCommandBuffer* commandBuffer, VkQueue* graphicsQueue);

// Added in 0.1
CALLISTO_CORE_RESULT CC_VK_submit_queue(VkCommandBuffer* commandBuffer, CCuint32 commandBufferCount, VkSemaphore* waitSemaphore, CCuint32 waitSemaphoreCount, VkSemaphore* signalSemaphore, CCuint32 signalSemaphoreCount, const VkPipelineStageFlags* pipelineStageFlags, VkFence* inFlightFence, VkQueue* graphicsQueue);

// Added in 0.1
CALLISTO_CORE_RESULT CC_VK_present_queue(VkSwapchainKHR* swapchain, VkQueue* presentQueue, VkSemaphore* signalSemaphore, CCuint32 imageIndex);


// Added in 0.1
void CC_VK_wait_device_idle(VkDevice *logicalDevice);

// Added in 0.1
void CC_VK_wait_queue_idle(VkQueue* queue);

//<<----- Vulkan ------>//

//<<----- Public Function Declarations ------>//