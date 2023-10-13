#include "CallistoCore.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

//<<----- Structs ------>//

struct p_CC_VK_PhysicalDeviceQueueFamilies {
	std::optional<CCuint32> graphicsFamily;
	std::optional<CCuint32> computeFamily;
	std::optional<CCuint32> transferFamily;

	std::optional<CCuint32> presentFamily;

	CCboolean p_CC_VK_physical_device_has_families() {
		return graphicsFamily.has_value() && computeFamily.has_value() && transferFamily.has_value() && presentFamily.has_value();
 	}
};

struct p_CC_VK_SwapchainDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

//<<----- Structs ------>//

//<<----- Internal Functions ------>//

//<<----- Vulkan - Internal ------>//

CCboolean p_CC_VK_isValidationLayerSupported(const std::vector<CCwords>& validationLayers) {
	CCuint32 layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for (CCwords layerName : validationLayers) {
		CCboolean layerFound = false;

		for (const auto& layerProperties : availableLayers) {
			if (strcmp(layerName, layerProperties.layerName) == 0) {
				layerFound = true;
				break;
			}
		}

		if (!layerFound) {
			return false;
		}
	}

	return true;
}

VKAPI_ATTR VkBool32 VKAPI_CALL p_CC_VK_debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
	std::cerr << "[Validation Layer]: " << pCallbackData->pMessage << std::endl;

	return VK_FALSE;
}


void p_CC_VK_device_family_checker(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, p_CC_VK_PhysicalDeviceQueueFamilies* physicalDeviceQueueFamilies) {
	CCuint32 queueFamiliesCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamiliesCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamiliesProperty(queueFamiliesCount);
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamiliesCount, queueFamiliesProperty.data());

	CCint i = 0;
	for (const auto& queueFamily : queueFamiliesProperty) {
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			physicalDeviceQueueFamilies->graphicsFamily = i;
		if (queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT)
			physicalDeviceQueueFamilies->computeFamily = i + 1;
		if (queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT)
			physicalDeviceQueueFamilies->transferFamily = i + 2;

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &presentSupport);

		if (presentSupport)
			physicalDeviceQueueFamilies->presentFamily = i;

		if (physicalDeviceQueueFamilies->p_CC_VK_physical_device_has_families())
			break;

		i++;
	}
}

CCboolean p_CC_VK_are_device_extensions_supported(VkPhysicalDevice physicalDevice, const std::vector<CCwords>& deviceExtensions) {
	CCuint32 extensionsCount;
	vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionsCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionsCount);
	vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionsCount, availableExtensions.data());

	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

	for (const auto& extension : availableExtensions) {
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}

void p_CC_VK_get_swapchain_details(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, p_CC_VK_SwapchainDetails* details) {
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &details->capabilities);

	CCuint32 formatsCount = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatsCount, nullptr);
	if (formatsCount != 0) {
		details->formats.resize(formatsCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatsCount, details->formats.data());
	}

	CCuint32 presentModesCount = 0;
	vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModesCount, nullptr);
	if (presentModesCount != 0) {
		details->presentModes.resize(presentModesCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModesCount, details->presentModes.data());
	}
}

CCboolean p_CC_VK_does_device_have_required_families(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, const std::vector<CCwords>& deviceExtensions) {
	p_CC_VK_PhysicalDeviceQueueFamilies physicalDeviceQueueFamilies;
	p_CC_VK_device_family_checker(physicalDevice, surface, &physicalDeviceQueueFamilies);

	CCboolean deviceExtensionsSupported = p_CC_VK_are_device_extensions_supported(physicalDevice, deviceExtensions);

	CCboolean swapchainAdequate = false;

	if (deviceExtensionsSupported) {
		p_CC_VK_SwapchainDetails swapchainSupport;
		p_CC_VK_get_swapchain_details(physicalDevice, surface, &swapchainSupport);
		swapchainAdequate = !swapchainSupport.formats.empty() && !swapchainSupport.presentModes.empty();
	}

	return physicalDeviceQueueFamilies.p_CC_VK_physical_device_has_families() && deviceExtensionsSupported && swapchainAdequate;
}

VkSurfaceFormatKHR p_CC_VK_choose_swapchain_format(const std::vector<VkSurfaceFormatKHR>& formats, VkFormat colorFormat, VkColorSpaceKHR colorSpace) {
	for (const auto& f : formats) {
		if (f.format == colorFormat && f.colorSpace == colorSpace) {
			return f;
		}
	}
	return formats[0];
}

VkPresentModeKHR p_CC_VK_choose_swapchain_present_mode(const std::vector<VkPresentModeKHR>& presentModes, VkPresentModeKHR wantedPresentMode, VkPresentModeKHR defaultPresentMode) {
	for (const auto& pM : presentModes) {
		if (pM == wantedPresentMode) {
			return pM;
		}
	}
	return defaultPresentMode;
}

VkExtent2D p_CC_VK_choose_swapchain_extent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window) {
	if (capabilities.currentExtent.width != std::numeric_limits<CCuint32>::max())
		return capabilities.currentExtent;
	else {
		CCint width, height;
		glfwGetFramebufferSize(window, &width, &height);

		VkExtent2D extent = {
			static_cast<CCuint32>(width),
			static_cast<CCuint32>(height)
		};

		extent.width = std::clamp(extent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
		extent.width = std::clamp(extent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

		return extent;
	}
 }

CCuint32 p_CC_VK_find_mem_type_index(VkPhysicalDevice* physicalDevice, CCuint32 typeFilter, VkMemoryPropertyFlags properties) {
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(*physicalDevice, &memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
			return i;
		}
	}

	std::cout << "Mem index not available" << '\n';
	return UINT32_MAX;
}

//<<----- Vulkan - Internal ------>//

//<<----- Internal Functions ------>//

//<<----- Public Functions ------>//

//<<----- Util Functions ------>//

void CC_get_version() {
	std::cout << "Callisto Core Version: " << MAJOR_VERSION << "." << MINOR_VERSION << "." << PATCH_VERSION << '\n';
}

//<<----- Util Functions ------>//

//<<----- GLFW ------>//

CALLISTO_CORE_RESULT CC_init_glfw() {
	if (glfwInit()) {
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		return CC_GLFW_INIT_SUCCESS;
	}
	return CC_GLFW_INIT_FAILURE;
}

void CC_set_window_attrib_init(CCint hint, CCint value) {
	glfwWindowHint(hint, value);
}

CALLISTO_CORE_RESULT CC_create_window(GLFWwindow** window, CCint width, CCint height, CCwords title) {
	*window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if (*window == nullptr) {
		return CC_GLFW_CREATE_WINDOW_FAILURE;
	}
	return CC_GLFW_CREATE_WINDOW_SUCCESS;
}

void CC_set_window_attrib_runtime(GLFWwindow** window, CCint attrib, CCint value) {
	glfwSetWindowAttrib(*window, attrib, value);
}

CCboolean CC_window_closing_status(GLFWwindow** window) {
	return glfwWindowShouldClose(*window);
}

void CC_poll_events() {
	glfwPollEvents();
}

void CC_framebuffer_size_change_callback(GLFWwindow** window, GLFWframebuffersizefun func) {
	glfwSetFramebufferSizeCallback(*window, func);
}

void CC_set_keyboard_callback(GLFWwindow** window, GLFWkeyfun func) {
	glfwSetKeyCallback(*window, func);
}

void CC_set_mouse_callback(GLFWwindow** window, GLFWmousebuttonfun func) {
	glfwSetMouseButtonCallback(*window, func);
}

void CC_set_mouse_pos_callback(GLFWwindow** window, GLFWcursorposfun func) {
	glfwSetCursorPosCallback(*window, func);
}

void CC_set_scroll_wheel_callback(GLFWwindow** window, GLFWscrollfun func) {
	glfwSetScrollCallback(*window, func);
}

CCboolean CC_get_keyboard_input(CCint key, CCboolean* keys) {
	return keys[key];
}

CCboolean CC_get_mouse_input(CCint button, CCboolean* buttons) {
	return buttons[button];
}

void CC_get_window_size(GLFWwindow** window, CCint* width, CCint* height) {
	glfwGetWindowSize(*window, width, height);
}

void CC_clock(CCdouble* sec) {
	*sec = glfwGetTime();
}


void CC_framerate(CCdouble* prevTime, CCint* frames, CCdouble* framerate, CCdouble* frametimes) {
	(*frames)++;
	CCdouble currentFrameTime = glfwGetTime();
	double deltaTime = currentFrameTime - (*prevTime);

	if (deltaTime >= (1.0 / 10.0)) {
		*framerate = (1.0f / deltaTime) * (*frames);

		*frametimes = (deltaTime / *frames) * 1000;

		*prevTime = currentFrameTime;
		currentFrameTime = glfwGetTime();
		(*frames) = 0;
	}
}

void CC_set_window_title(GLFWwindow** window, CCwords title) {
	glfwSetWindowTitle(*window, title);
}

void CC_set_window_size(GLFWwindow** window, CCint width, CCint height) {
	glfwSetWindowSize(*window, width, height);
}

void CC_set_window_position(GLFWwindow** window, CCint xPos, CCint yPos) {
	glfwSetWindowPos(*window, xPos, yPos);
}

void CC_set_window_opacity(GLFWwindow** window, CCint opacity) {
	assert(opacity <= 255);
	CCfloat normalizedOpacity = opacity / 255.0f;
	glfwSetWindowOpacity(*window, normalizedOpacity);
}

void CC_set_window_icon(GLFWwindow** window, CCwords filename) {
	GLFWimage* icon{};
	CCint width, height, channels;
	stbi_uc* imageData = stbi_load(filename, &width, &height, &channels, 0);
	icon->width = width;
	icon->height = height; 
	icon->pixels = imageData;
	glfwSetWindowIcon(*window, 1, icon);
	stbi_image_free(imageData);
}

void CC_destroy_window(GLFWwindow** window) {
	glfwDestroyWindow(*window);
}

void CC_terminate_glfw() {
	glfwTerminate();
}

//<<----- GLFW ------>//

//<<----- FMOD ------>//

#include <FMOD/fmod_errors.h>

CALLISTO_CORE_RESULT CC_FMD_create_sound_system(FMOD::System** system) {
	if (FMOD::System_Create(system) == FMOD_OK)
		return CC_FMD_SOUND_SYSTEM_CREATION_SUCCESS;
	else
		return CC_FMD_SOUND_SYSTEM_CREATION_FAILURE;
}

CALLISTO_CORE_RESULT CC_FMD_init_sound_system(FMOD::System* system, CCint maxChannels, FMOD_INITFLAGS flags) {
	if (system->init(maxChannels, flags, nullptr) == FMOD_OK)
		return CC_FMD_SOUND_SYSTEM_INITIALIZATION_SUCCESS;
	else
		return CC_FMD_SOUND_SYSTEM_INITIALIZATION_FAILURE;
}

CALLISTO_CORE_RESULT CC_FMD_create_sound(FMOD::System* system, FMOD::Sound** sound, CCwords soundFile, FMOD_MODE mode) {
	if (system->createSound(soundFile, mode, 0, sound) == FMOD_OK)
		return CC_FMD_SOUND_CREATION_SUCCESS;
	else
		return CC_FMD_SOUND_CREATION_FAILURE;
}

void CC_FMD_set_position(FMOD::Channel* channel, const FMOD_VECTOR* position, const FMOD_VECTOR* listenerOrientation) {
	channel->set3DAttributes(position, listenerOrientation);
}

void CC_FMD_get_position(FMOD::Channel* channel, FMOD_VECTOR* position, FMOD_VECTOR* listenerOrientation) {
	channel->get3DAttributes(position, listenerOrientation);
}

void CC_FMD_set_pan(FMOD::Channel* channel, CCfloat pan) {
	assert((pan <= 100.0f) && (pan >= -100.0f));
	CCfloat normalizedPan = pan / 100.0f;
	channel->setPan(normalizedPan);
}

void CC_FMD_set_pitch(FMOD::Channel* channel, CCfloat pitch) {
	assert((pitch <= 100.0f) && (pitch >= 0.0f));
	CCfloat normalizedPit = pitch / 100.0f;
	channel->setPitch(normalizedPit);
}

void CC_FMD_get_pitch(FMOD::Channel* channel, CCfloat* pitch) {
	CCfloat normalizedPit = 0;
	channel->getPitch(&normalizedPit);
	*pitch = normalizedPit * 100.0f;
}

void CC_FMD_set_volume(FMOD::Channel* channel, CCfloat volume) {
	assert((volume <= 100.0f) && (volume >= 0.0f));
	CCfloat normalizedVol = volume / 100.0f;
	channel->setVolume(normalizedVol);
}

void CC_FMD_get_volume(FMOD::Channel* channel, CCfloat* volume) {
	CCfloat normalizedVol = 0;
	channel->getVolume(&normalizedVol);
	*volume = normalizedVol * 100.0f;
}

void CC_FMD_set_priority(FMOD::Channel* channel, CCint priority) {
	channel->setPriority(priority);
}

void CC_FMD_get_prioirty(FMOD::Channel* channel, CCint* priority) {
	channel->getPriority(priority);
}

void CC_FMD_set_paused(FMOD::Channel* channel, CCboolean paused) {
	channel->setPaused(paused);
}

void CC_FMD_get_paused(FMOD::Channel* channel, CCboolean* paused) {
	channel->getPaused(paused);
}

void CC_FMD_set_mute(FMOD::Channel* channel, CCboolean mute) {
	channel->setMute(mute);
}

void CC_FMD_get_mute(FMOD::Channel* channel, CCboolean* mute) {
	channel->getMute(mute);
}

void CC_FMD_set_mode(FMOD::Channel* channel, FMOD_MODE mode) {
	channel->setMode(mode);
}

void CC_FMD_get_mode(FMOD::Channel* channel, FMOD_MODE* mode) {
	channel->getMode(mode);
}

void CC_FMD_set_timestamp(FMOD::Channel* channel, CCuint32 time, FMOD_TIMEUNIT timeMeasurement) {
	channel->setPosition(time, timeMeasurement);
}

void CC_FMD_get_timestamp(FMOD::Channel* channel, CCuint32* time, FMOD_TIMEUNIT timeMeasurement) {
	channel->getPosition(time, timeMeasurement);
}

void CC_FMD_play_sound(FMOD::System* system, FMOD::Sound* sound, FMOD::ChannelGroup* channelGroup, FMOD::Channel** channel, CCboolean paused) {
	system->playSound(sound, channelGroup, paused, channel);
}

void CC_FMD_destroy_sound(FMOD::Sound* sound) {
	sound->release();
}

void CC_FMD_destroy_sound_system(FMOD::System* system) {
	system->close();
	system->release();
}

//<<----- FMOD ------>//

//<<----- FreeType ----->//

CALLISTO_CORE_RESULT CC_FT_init(FT_Library* ft) {
	if (FT_Init_FreeType(ft) == FT_Err_Ok)
		return CC_FT_INITIALIZED_SUCCESSFULLY;
	else
		return CC_FT_INITIALIZED_UNSUCCESSFULLY;
}

CALLISTO_CORE_RESULT CC_FT_create_face(FT_Library* ft, FT_Face* face, CCwords fontFile) {
	if (FT_New_Face(*ft, fontFile, 0, face) == FT_Err_Ok)
		return CC_FT_FACE_CREATION_SUCCESS;
	else
		return CT_FT_FACE_CREATION_FAILED;
}

void CC_FT_set_pixel_size(FT_Face* face, CCuint32 fontWidthPX, CCuint32 fontHeightPX) {
	FT_Set_Pixel_Sizes(*face, fontWidthPX, fontHeightPX);
}

void CC_FT_load_char(FT_Face* face, CCulong charCode, CCint flags) {
	FT_Load_Char(*face, charCode, flags);
}

void CC_FT_destroy_face(FT_Face* face) {
	FT_Done_Face(*face);
}

void CC_FT_un_init(FT_Library* ft) {
	FT_Done_FreeType(*ft);
}

//<<----- FreeType ----->//

//<<----- Vulkan ------>//

void CC_VK_create_app_info(VkApplicationInfo* appInfo, CCwords appName, CCwords engineName, std::array<CCint, 3> appVersion, std::array<CCint, 3> engineVersion, CCuint32 vulkanApiVersion) {
	appInfo->sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo->pApplicationName = appName;
	appInfo->applicationVersion = VK_MAKE_VERSION(appVersion[0], appVersion[1], appVersion[2]);
	appInfo->pEngineName = engineName;
	appInfo->engineVersion = VK_MAKE_VERSION(appVersion[0], appVersion[1], appVersion[2]);
	appInfo->apiVersion = vulkanApiVersion;
}

CALLISTO_CORE_RESULT CC_VK_create_instance(VkInstance* instance, VkApplicationInfo* appInfo, VkDebugUtilsMessengerEXT* debugMessenger, const std::vector<CCwords>& validationLayers, CCboolean validationLayerToggle) {
	if (validationLayerToggle && !p_CC_VK_isValidationLayerSupported(validationLayers)) return CC_VK_VALIDATION_LAYERS_NOT_FOUND;
	
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	
	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
	if (validationLayerToggle) {
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}
	
	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = appInfo;
	createInfo.enabledExtensionCount = static_cast<CCuint32>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();
	
	VkDebugUtilsMessengerCreateInfoEXT validCreateInfo{};
	if (validationLayerToggle) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();

		validCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		validCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		validCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;


		validCreateInfo.pfnUserCallback = p_CC_VK_debugCallback;

		createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&validCreateInfo;
	}
	else {
		createInfo.enabledLayerCount = 0;

		createInfo.pNext = nullptr;
	}

	if (vkCreateInstance(&createInfo, nullptr, instance) == VK_SUCCESS) {
		if (validationLayerToggle) {
			auto vkCreateDebugUtilsMessengerEXT = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(*instance, "vkCreateDebugUtilsMessengerEXT");
			if (vkCreateDebugUtilsMessengerEXT(*instance, &validCreateInfo, nullptr, debugMessenger) == VK_SUCCESS)
				return CC_VK_INSTANCE_CREATION_SUCCESS;
			else
				return CC_VK_VALIDATION_CREATION_FAILURE;
		}
		return CC_VK_INSTANCE_CREATION_SUCCESS;
	}
	else return CC_VK_INSTANCE_CREATION_FAILURE;
}

void CC_VK_destroy_instance(VkInstance* instance, VkDebugUtilsMessengerEXT* debugMessenger, CCboolean validationLayerToggle) {
	auto vkDestroyDebugUtilsMessengerEXT = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(*instance, "vkDestroyDebugUtilsMessengerEXT");
	if (validationLayerToggle)
		vkDestroyDebugUtilsMessengerEXT(*instance, *debugMessenger, nullptr);
	vkDestroyInstance(*instance, nullptr);
}

CALLISTO_CORE_RESULT CC_VK_locate_physical_device(VkInstance* instance, VkPhysicalDevice* physicalDevice, VkSurfaceKHR* surface, const std::vector<CCwords>& deviceExtensions) {
	CCuint32 physicalDeviceCount = 0;
	vkEnumeratePhysicalDevices(*instance, &physicalDeviceCount, nullptr);

	if (physicalDeviceCount == 0) return CC_VK_NO_PHYSICAL_DEVICE_FOUND;

	std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
	vkEnumeratePhysicalDevices(*instance, &physicalDeviceCount, physicalDevices.data());

	for (const auto& device : physicalDevices) {
		if (p_CC_VK_does_device_have_required_families(device, *surface, deviceExtensions)) {
			*physicalDevice = device;
			return CC_VK_PHYSICAL_DEVICE_WITH_NECESSARY_QUEUE_FAMILIES_FOUND;
		}
	}
	return CC_VK_PHYSICAL_DEVICE_WITH_NECESSARY_QUEUE_FAMILIES_NOT_FOUND;
}

CALLISTO_CORE_RESULT CC_VK_create_logical_device(VkInstance* instance, VkPhysicalDevice* physicalDevice, VkDevice* logicalDevice, VkSurfaceKHR* surface, VkQueue* graphicsQueue, VkQueue* computeQueue, VkQueue* transferQueue, VkQueue* presentQueue, const std::vector<CCwords>& validationLayers, const std::vector<CCwords>& deviceExtensions,CCboolean validationLayerToggle) {
	p_CC_VK_PhysicalDeviceQueueFamilies physicalDeviceQueueFamilies;
	p_CC_VK_device_family_checker(*physicalDevice, *surface, &physicalDeviceQueueFamilies);

	CCfloat graphicsQueuePriority = 1.0f;
	CCfloat computeQueuePriority = 0.6f;
	CCfloat transferQueuePriority = 0.3f;

	VkDeviceQueueCreateInfo queueCreateInfo[3]{};
	
	queueCreateInfo[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueCreateInfo[0].queueFamilyIndex = physicalDeviceQueueFamilies.graphicsFamily.value();
	queueCreateInfo[0].queueCount = 1;
	queueCreateInfo[0].pQueuePriorities = &graphicsQueuePriority;
	
	queueCreateInfo[1].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueCreateInfo[1].queueFamilyIndex = physicalDeviceQueueFamilies.computeFamily.value();
	queueCreateInfo[1].queueCount = 1;
	queueCreateInfo[1].pQueuePriorities = &computeQueuePriority;
	
	queueCreateInfo[2].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueCreateInfo[2].queueFamilyIndex = physicalDeviceQueueFamilies.transferFamily.value();
	queueCreateInfo[2].queueCount = 1;
	queueCreateInfo[2].pQueuePriorities = &transferQueuePriority;

	VkPhysicalDeviceFeatures deviceFeatures{};

	VkDeviceCreateInfo deviceCreateInfo{};
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateInfo.pQueueCreateInfos = queueCreateInfo;
	deviceCreateInfo.queueCreateInfoCount = 3;
	deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
	deviceCreateInfo.enabledExtensionCount = static_cast<CCuint32>(deviceExtensions.size());
	deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();

	if (validationLayerToggle) {
		deviceCreateInfo.enabledLayerCount = static_cast<CCuint32>(validationLayers.size());
		deviceCreateInfo.ppEnabledLayerNames = validationLayers.data();
	}
	else {
		deviceCreateInfo.enabledLayerCount = 0;
	}

	if (vkCreateDevice(*physicalDevice, &deviceCreateInfo, nullptr, logicalDevice) == VK_SUCCESS) {
		vkGetDeviceQueue(*logicalDevice, physicalDeviceQueueFamilies.graphicsFamily.value(), 0, graphicsQueue);
		vkGetDeviceQueue(*logicalDevice, physicalDeviceQueueFamilies.computeFamily.value(), 0, computeQueue);
		vkGetDeviceQueue(*logicalDevice, physicalDeviceQueueFamilies.transferFamily.value(), 0, transferQueue);
		vkGetDeviceQueue(*logicalDevice, physicalDeviceQueueFamilies.presentFamily.value(), 0, presentQueue);
		return CC_VK_LOGICAL_DEVICE_CREATION_SUCCESS;
	}
	return CC_VK_LOGICAL_DEVICE_CREATION_FAILURE;
}

void CC_VK_destroy_logical_device(VkDevice* logicalDevice) {
	vkDestroyDevice(*logicalDevice, nullptr);
}

CALLISTO_CORE_RESULT CC_VK_create_surface(VkInstance* instance, GLFWwindow** window, VkSurfaceKHR* surface) {
	if (glfwCreateWindowSurface(*instance, *window, nullptr, surface) == VK_SUCCESS)
		return CC_VK_SURFACE_CREATION_SUCCESS;
	return CC_VK_SURFACE_CREATION_FAILURE;
}

void CC_VK_destroy_surface(VkInstance* instance, VkSurfaceKHR* surface) {
	vkDestroySurfaceKHR(*instance, *surface, nullptr);
}

CALLISTO_CORE_RESULT CC_VK_create_swapchain(VkPhysicalDevice* physicalDevice, VkDevice* logicalDevice, VkSurfaceKHR* surface, GLFWwindow** window, VkSwapchainKHR* swapchain, std::vector<VkImage>& swapchainImages, VkFormat* swapchainImageFormat, VkExtent2D* swapchainExtent, VkFormat colorFormat, VkColorSpaceKHR colorSpace, VkPresentModeKHR wantedPresentMode, VkPresentModeKHR defaultPresentMode, VkImageUsageFlagBits imageUsage, VkCompositeAlphaFlagBitsKHR compositeAlpha) {
	p_CC_VK_SwapchainDetails swapchainDetails;
	p_CC_VK_get_swapchain_details(*physicalDevice, *surface, &swapchainDetails);

	VkSurfaceFormatKHR surfaceFormat = p_CC_VK_choose_swapchain_format(swapchainDetails.formats, colorFormat, colorSpace);
	VkPresentModeKHR presentMode = p_CC_VK_choose_swapchain_present_mode(swapchainDetails.presentModes, wantedPresentMode, defaultPresentMode);
	VkExtent2D extent = p_CC_VK_choose_swapchain_extent(swapchainDetails.capabilities, *window);

	CCuint32 imageCount = swapchainDetails.capabilities.minImageCount + 1;
	if (swapchainDetails.capabilities.maxImageCount > 0 && imageCount > swapchainDetails.capabilities.maxImageCount)
		imageCount = swapchainDetails.capabilities.maxImageCount;

	VkSwapchainCreateInfoKHR swapchainCreateInfo{};
	swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapchainCreateInfo.surface = *surface;
	swapchainCreateInfo.minImageCount = imageCount;
	swapchainCreateInfo.imageFormat = surfaceFormat.format;
	swapchainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
	swapchainCreateInfo.imageExtent = extent;
	swapchainCreateInfo.imageArrayLayers = 1;
	swapchainCreateInfo.imageUsage = imageUsage;

	p_CC_VK_PhysicalDeviceQueueFamilies deviceQueueFamilies;
	p_CC_VK_device_family_checker(*physicalDevice, *surface, &deviceQueueFamilies);
	CCuint32 queueFamilyIndices[] = { deviceQueueFamilies.graphicsFamily.value(), deviceQueueFamilies.presentFamily.value() };

	if (deviceQueueFamilies.graphicsFamily != deviceQueueFamilies.presentFamily) {
		swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		swapchainCreateInfo.queueFamilyIndexCount = 2;
		swapchainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else {
		swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	}

	swapchainCreateInfo.preTransform = swapchainDetails.capabilities.currentTransform;
	swapchainCreateInfo.compositeAlpha = compositeAlpha;
	swapchainCreateInfo.presentMode = presentMode;
	swapchainCreateInfo.clipped = VK_TRUE;

	if (vkCreateSwapchainKHR(*logicalDevice, &swapchainCreateInfo, nullptr, swapchain) == VK_SUCCESS) {
		vkGetSwapchainImagesKHR(*logicalDevice, *swapchain, &imageCount, nullptr);
		swapchainImages.resize(imageCount);
		vkGetSwapchainImagesKHR(*logicalDevice, *swapchain, &imageCount, swapchainImages.data());
	
		*swapchainImageFormat = surfaceFormat.format;
		*swapchainExtent = extent;
	
		return CC_VK_SWAPCHAIN_CREATION_SUCCESS;
	}
	else 
		return CC_VK_SWAPCHAIN_CREATION_FAILURE;

}

void CC_VK_destroy_swapchain(VkDevice* logicalDevice, VkSwapchainKHR* swapchain) {
	vkDestroySwapchainKHR(*logicalDevice, *swapchain, nullptr);
}

CALLISTO_CORE_RESULT CC_VK_create_image_views(VkDevice *logicalDevice, std::vector<VkImageView>& swapchainImageViews, std::vector<VkImage>& swapchainImages, VkFormat* swapchainImageFormat, VkImageViewType imageViewType, VkComponentSwizzle redSwizzle, VkComponentSwizzle greenSwizzle, VkComponentSwizzle blueSwizzle, VkComponentSwizzle alphaSwizzle, VkImageAspectFlagBits aspectMask) {
	swapchainImageViews.resize(swapchainImages.size());
	
	for (int img = 0; img < swapchainImages.size(); img++) {
		VkImageViewCreateInfo imageViewInfo{};
		imageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageViewInfo.image = swapchainImages[img];
		imageViewInfo.viewType = imageViewType;
		imageViewInfo.format = *swapchainImageFormat;
		imageViewInfo.components.r = redSwizzle;
		imageViewInfo.components.g = greenSwizzle;
		imageViewInfo.components.b = blueSwizzle;
		imageViewInfo.components.a = alphaSwizzle;
		imageViewInfo.subresourceRange.aspectMask = aspectMask;
		imageViewInfo.subresourceRange.baseMipLevel = 0;
		imageViewInfo.subresourceRange.levelCount = 1;
		imageViewInfo.subresourceRange.baseArrayLayer = 0;
		imageViewInfo.subresourceRange.layerCount = 1;

		if (vkCreateImageView(*logicalDevice, &imageViewInfo, nullptr, &swapchainImageViews[img]) != VK_SUCCESS) {
			return CC_VK_SWAPCHAIN_IMAGE_VIEWS_CREATION_FAILURE;
		}
	}
	return CC_VK_SWAPCHAIN_IMAGE_VIEWS_CREATION_SUCCESS;
}

void CC_VK_destroy_image_views(VkDevice* logicalDevice, std::vector<VkImageView>& swapchainImageViews) {
	for (int img = 0; img < swapchainImageViews.size(); img++) {
		vkDestroyImageView(*logicalDevice, swapchainImageViews[img], nullptr);
	}
}

void CC_VK_color_attachment_desc(VkAttachmentDescription* colorAttachmentDesc, VkFormat* format, VkSampleCountFlagBits samples, VkAttachmentLoadOp loadOp, VkAttachmentStoreOp storeOp, VkAttachmentLoadOp stencilLoadOp, VkAttachmentStoreOp stencilStoreOp, VkImageLayout imageLayout, VkImageLayout finalLayout) {
	colorAttachmentDesc->format = *format;
	colorAttachmentDesc->samples = samples;
	colorAttachmentDesc->loadOp = loadOp;
	colorAttachmentDesc->storeOp = storeOp;
	colorAttachmentDesc->stencilLoadOp = stencilLoadOp;
	colorAttachmentDesc->stencilStoreOp = stencilStoreOp;
	colorAttachmentDesc->initialLayout = imageLayout;
	colorAttachmentDesc->finalLayout = finalLayout;
}

void CC_VK_color_attachment_ref(VkAttachmentReference* colorAttachmentRef, CCuint32 attachment, VkImageLayout layout) {
	colorAttachmentRef->attachment = attachment;
	colorAttachmentRef->layout = layout;
}

void CC_VK_subpass_desc(VkSubpassDescription* subpassDesc, VkPipelineBindPoint pipelineBindPoint, CCuint32 colorAttachmentCount, const VkAttachmentReference* pColorAttachments) {
	subpassDesc->pipelineBindPoint = pipelineBindPoint;
	subpassDesc->colorAttachmentCount = colorAttachmentCount;
	subpassDesc->pColorAttachments = pColorAttachments;
}

CALLISTO_CORE_RESULT CC_VK_create_renderpass(VkRenderPass* renderpass, VkDevice* logicalDevice, CCuint32 attachmentCount,  const VkAttachmentDescription* colorAttachmentDesc, CCuint32 subpassCount, const VkSubpassDescription* subpassDesc) {
	VkRenderPassCreateInfo renderpassCreateInfo{};
	renderpassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderpassCreateInfo.attachmentCount = attachmentCount;
	renderpassCreateInfo.pAttachments = colorAttachmentDesc;
	renderpassCreateInfo.subpassCount = subpassCount;
	renderpassCreateInfo.pSubpasses = subpassDesc;

	if (vkCreateRenderPass(*logicalDevice, &renderpassCreateInfo, nullptr, renderpass) == VK_SUCCESS)
		return CC_VK_RENDER_PASS_CREATION_SUCCESS;
	else
		return CC_VK_RENDER_PASS_CREATION_FAILURE;
}

void CC_VK_destroy_renderpass(VkDevice* logicalDevice, VkRenderPass* renderpass) {
	vkDestroyRenderPass(*logicalDevice, *renderpass, nullptr);
}

void CC_VK_descriptor_set_layout_binding(VkDescriptorSetLayoutBinding* descSetLayBind, CCuint32 binding, CCuint32 descriptorCount, VkDescriptorType descriptorType, VkSampler* immutableSamplers, VkShaderStageFlags stageFlags) {
	descSetLayBind->binding = binding;
	descSetLayBind->descriptorCount = descriptorCount;
	descSetLayBind->descriptorType = descriptorType;
	descSetLayBind->pImmutableSamplers = immutableSamplers;
	descSetLayBind->stageFlags = stageFlags;
}

CALLISTO_CORE_RESULT CC_VK_create_descriptor_set_layout(VkDevice* logicalDevice, VkDescriptorSetLayout* descSetLayout, CCuint32 bindingCount, const VkDescriptorSetLayoutBinding* bindings) {
	VkDescriptorSetLayoutCreateInfo descSetLayCreateInfo{};
	descSetLayCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	descSetLayCreateInfo.bindingCount = bindingCount;
	descSetLayCreateInfo.pBindings = bindings;

	if (vkCreateDescriptorSetLayout(*logicalDevice, &descSetLayCreateInfo, nullptr, descSetLayout) == VK_SUCCESS)
		return CC_VK_DESCRIPTOR_SET_LAYOUT_CREATION_SUCCESS;
	else
		return CC_VK_DESCRIPTOR_SET_LAYOUT_CREATION_FAILURE;
}

void CC_VK_create_descriptor_pool_size(VkDescriptorPoolSize* descPoolSize, VkDescriptorType type, CCuint32 descriptorCount) {
	descPoolSize->type = type;
	descPoolSize->descriptorCount = descriptorCount;
}

CALLISTO_CORE_RESULT CC_VK_create_descriptor_pool(VkDevice* logicalDevice, VkDescriptorPool *descPool, CCuint32 poolSizeCount, const VkDescriptorPoolSize* poolSizes, CCuint32 maxSets) {
	VkDescriptorPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = poolSizeCount;
	poolInfo.pPoolSizes = poolSizes;
	poolInfo.maxSets = maxSets;

	if (vkCreateDescriptorPool(*logicalDevice, &poolInfo, nullptr, descPool) == VK_SUCCESS)
		return CC_VK_DESCRIPTOR_POOL_CREATION_SUCCESS;
	else
		return CC_VK_DESCRIPTOR_POOL_CREATION_FAILURE;
}

CALLISTO_CORE_RESULT CC_VK_create_descriptor_sets(VkDevice* logicalDevice, VkDescriptorSet* descSet, VkDescriptorPool* descPool, VkDescriptorSetLayout* descSetLayout) {
	VkDescriptorSetAllocateInfo allocDescSetInfo{};
	allocDescSetInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocDescSetInfo.descriptorPool = *descPool;
	allocDescSetInfo.descriptorSetCount = 1;
	allocDescSetInfo.pSetLayouts = descSetLayout;

	if (vkAllocateDescriptorSets(*logicalDevice, &allocDescSetInfo, descSet) == VK_SUCCESS)
		return CC_VK_DESCRIPTOR_SET_CREATION_SUCCESS;
	else
		return CC_VK_DESCRIPTOR_SET_CREATION_FAILURE;
}

void CC_VK_get_physical_device_properties(VkPhysicalDeviceProperties* physDevProp, VkPhysicalDevice* physicalDevice) {
	vkGetPhysicalDeviceProperties(*physicalDevice, physDevProp);
}

void CC_VK_descriptor_buffer_info(VkDescriptorBufferInfo* bufferInfo, VkBuffer buffer, VkDeviceSize offset, VkDeviceSize range) {
	bufferInfo->buffer = buffer;
	bufferInfo->offset = offset;
	bufferInfo->range = range;
}

void CC_VK_descriptor_image_info(VkDescriptorImageInfo* bufferInfo, VkImageLayout imageLayout, VkImageView imageView, VkSampler sampler) {
	bufferInfo->imageView = imageView;
	bufferInfo->imageLayout = imageLayout;
	bufferInfo->sampler = sampler;
}


void CC_VK_write_descriptor_sets_buffer(VkDescriptorSet* descSet, VkWriteDescriptorSet* descWrite, CCuint32 dstBinding, CCuint32 dstArrayElement, VkDescriptorType descriptorType, CCuint32 descriptorCount, const VkDescriptorBufferInfo* bufferInfo) {
	descWrite->sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descWrite->dstSet = *descSet;
	descWrite->dstBinding = dstBinding;
	descWrite->dstArrayElement = dstArrayElement;
	descWrite->descriptorType = descriptorType;
	descWrite->descriptorCount = descriptorCount;
	descWrite->pBufferInfo = bufferInfo;
}

void CC_VK_write_descriptor_sets_image(VkDescriptorSet* descSet, VkWriteDescriptorSet* descWrite, CCuint32 dstBinding, CCuint32 dstArrayElement, VkDescriptorType descriptorType, CCuint32 descriptorCount, const VkDescriptorImageInfo* imageInfo) {
	descWrite->sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descWrite->dstSet = *descSet;
	descWrite->dstBinding = dstBinding;
	descWrite->dstArrayElement = dstArrayElement;
	descWrite->descriptorType = descriptorType;
	descWrite->descriptorCount = descriptorCount;
	descWrite->pImageInfo = imageInfo;
}

void CC_VK_update_descriptor_sets(VkDevice* logicalDevice, CCuint32 descWriteCount, const VkWriteDescriptorSet* descWrite, CCuint32 descCopyCount, const VkCopyDescriptorSet* descCopy) {
	vkUpdateDescriptorSets(*logicalDevice, descWriteCount, descWrite, descCopyCount, descCopy);
}

void CC_VK_destroy_descriptor_pool(VkDevice* logicalDevice, VkDescriptorPool* descPool) {
	vkDestroyDescriptorPool(*logicalDevice, *descPool, nullptr);
}

void CC_VK_destroy_descriptor_set_layout(VkDevice* logicalDevice, VkDescriptorSetLayout* descSetLayout) {
	vkDestroyDescriptorSetLayout(*logicalDevice, *descSetLayout, nullptr);
}

CALLISTO_CORE_RESULT CC_VK_create_shader_module(VkDevice* logicalDevice, VkShaderModule* shaderProgram, CCwords shaderCode, CCwords entryPointName, shaderc_shader_kind shaderType) {
	shaderc::Compiler compiler;
	shaderc::CompileOptions compileOption;
	shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(shaderCode, strlen(shaderCode), shaderType, "shader", entryPointName, compileOption);

	if (module.GetCompilationStatus() != shaderc_compilation_status_success)
		return CC_VK_SHADER_MODULE_COMPILATION_FAILURE;

	std::vector<CCuint32> shaderCodeSPIRV(module.cbegin(), module.cend());

	VkShaderModuleCreateInfo shaderCreateInfo{};
	shaderCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	shaderCreateInfo.codeSize = shaderCodeSPIRV.size() * sizeof(CCuint32);
	shaderCreateInfo.pCode = shaderCodeSPIRV.data();

	if (vkCreateShaderModule(*logicalDevice, &shaderCreateInfo, nullptr, shaderProgram) == VK_SUCCESS)
		return CC_VK_SHADER_MODULE_CREATION_SUCCESS;
	else
		return CC_VK_SHADER_MODULE_CREATION_FAILURE;
}

void CC_VK_create_graphics_pipeline_shader_stage(VkPipelineShaderStageCreateInfo* shaderStageCreateInfo, VkShaderStageFlagBits shaderStage, VkShaderModule* shaderModule, CCwords entryPoint) {
	shaderStageCreateInfo->sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shaderStageCreateInfo->stage = shaderStage;
	shaderStageCreateInfo->module = *shaderModule;
	shaderStageCreateInfo->pName = entryPoint;
}

void CC_VK_vertex_input_binding_desc(VkVertexInputBindingDescription* vertexInputBindDesc, CCuint32 binding, CCuint32 stride, VkVertexInputRate inputRate) {
	vertexInputBindDesc->binding = binding;
	vertexInputBindDesc->stride = stride;
	vertexInputBindDesc->inputRate = inputRate;
}

void CC_VK_vertex_input_attrib_desc(VkVertexInputAttributeDescription* vertexInputAttribDesc, CCuint32 binding, CCuint32 location, VkFormat format, CCuint32 offset) {
	vertexInputAttribDesc->binding = binding;
	vertexInputAttribDesc->location = location;
	vertexInputAttribDesc->format = format;
	vertexInputAttribDesc->offset = offset;
}

void CC_VK_pipe_vertex_input_state(VkPipelineVertexInputStateCreateInfo* vertexInputStateInfo, CCuint32 vertexInputBindDescCount, const VkVertexInputBindingDescription* vertexInputBindDesc, CCuint32 vertexInputAttribDescCount, const VkVertexInputAttributeDescription* vertexInputAttribDesc) {
	vertexInputStateInfo->sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputStateInfo->vertexBindingDescriptionCount = vertexInputBindDescCount;
	vertexInputStateInfo->pVertexBindingDescriptions = vertexInputBindDesc;
	vertexInputStateInfo->vertexAttributeDescriptionCount = vertexInputAttribDescCount;
	vertexInputStateInfo->pVertexAttributeDescriptions = vertexInputAttribDesc;
}

void CC_VK_pipe_input_assembly_state(VkPipelineInputAssemblyStateCreateInfo* inputAssemblyStateInfo, VkPrimitiveTopology topology, VkBool32 primitiveRestartEnable) {
	inputAssemblyStateInfo->sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssemblyStateInfo->topology = topology; 
	inputAssemblyStateInfo->primitiveRestartEnable = primitiveRestartEnable;
}

void CC_VK_pipe_viewport_state(VkPipelineViewportStateCreateInfo* viewportStateInfo, CCuint32 viewportCount, CCuint32 scissorCount) {
	viewportStateInfo->sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportStateInfo->viewportCount = viewportCount;
	viewportStateInfo->scissorCount = scissorCount;
}

void CC_VK_pipe_rasterization_state(VkPipelineRasterizationStateCreateInfo* rasterizerStateInfo, VkBool32 depthClampEnable, VkBool32 rasterizerDiscardEnable, VkPolygonMode polygonMode, CCfloat lineWidth, VkCullModeFlags cullMode, VkFrontFace frontFace, VkBool32 depthBiasEnable) {
	rasterizerStateInfo->sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizerStateInfo->depthClampEnable = depthBiasEnable;
	rasterizerStateInfo->rasterizerDiscardEnable = rasterizerDiscardEnable;
	rasterizerStateInfo->polygonMode = polygonMode;
	rasterizerStateInfo->lineWidth = lineWidth;
	rasterizerStateInfo->cullMode = cullMode;
	rasterizerStateInfo->frontFace = frontFace;
	rasterizerStateInfo->depthBiasEnable = depthBiasEnable;
}

void CC_VK_pipe_multisample_state(VkPipelineMultisampleStateCreateInfo* multisamplingStateInfo, VkBool32 sampleShadingEnable, VkSampleCountFlagBits rasterizationSamples) {
	multisamplingStateInfo->sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisamplingStateInfo->sampleShadingEnable = sampleShadingEnable;
	multisamplingStateInfo->rasterizationSamples = rasterizationSamples;
}

void CC_VK_pipe_colorblend_attachment_state(VkPipelineColorBlendAttachmentState* colorblendAttachmentState, VkColorComponentFlags colorWriteMask, VkBool32 blendEnable, VkBlendFactor srcColorBlendFactor, VkBlendFactor dstColorBlendFactor, VkBlendOp colorBlendOp, VkBlendFactor srcAlphaBlendFactor, VkBlendFactor dstAlphaBlendFactor, VkBlendOp alphaBlendOp) {
	colorblendAttachmentState->colorWriteMask = colorWriteMask;
	colorblendAttachmentState->blendEnable = blendEnable;
	colorblendAttachmentState->srcColorBlendFactor = srcColorBlendFactor;
	colorblendAttachmentState->dstColorBlendFactor = dstColorBlendFactor;
	colorblendAttachmentState->colorBlendOp = colorBlendOp;
	colorblendAttachmentState->srcAlphaBlendFactor = srcAlphaBlendFactor;
	colorblendAttachmentState->dstAlphaBlendFactor = dstAlphaBlendFactor;
	colorblendAttachmentState->alphaBlendOp = alphaBlendOp;
}

void CC_VK_pipe_colorblend_state(VkPipelineColorBlendStateCreateInfo* colorblendStateInfo, VkPipelineColorBlendAttachmentState* colorBlendAttachmentState, VkBool32 logicOpEnable, VkLogicOp logicOp, std::array<CCfloat, 4> blendConstants) {
	colorblendStateInfo->sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorblendStateInfo->logicOpEnable = logicOpEnable;
	colorblendStateInfo->logicOp = logicOp;
	colorblendStateInfo->attachmentCount = 1;
	colorblendStateInfo->pAttachments = colorBlendAttachmentState;
	colorblendStateInfo->blendConstants[0] = blendConstants[0];
	colorblendStateInfo->blendConstants[1] = blendConstants[1];
	colorblendStateInfo->blendConstants[2] = blendConstants[2];
	colorblendStateInfo->blendConstants[3] = blendConstants[3];
}

void CC_VK_pipe_colorblend_state(VkPipelineColorBlendStateCreateInfo* colorblendStateInfo, CCuint32 colorBlendAttachmentStatesCount, const VkPipelineColorBlendAttachmentState* colorBlendAttachmentStates, VkBool32 logicOpEnable, VkLogicOp logicOp, std::array<CCfloat, 4> blendConstants) {
	colorblendStateInfo->sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorblendStateInfo->logicOpEnable = logicOpEnable;
	colorblendStateInfo->logicOp = logicOp;
	colorblendStateInfo->attachmentCount = colorBlendAttachmentStatesCount;
	colorblendStateInfo->pAttachments = colorBlendAttachmentStates;
	colorblendStateInfo->blendConstants[0] = blendConstants[0];
	colorblendStateInfo->blendConstants[1] = blendConstants[1];
	colorblendStateInfo->blendConstants[2] = blendConstants[2];
	colorblendStateInfo->blendConstants[3] = blendConstants[3];  
}

void CC_VK_pipe_dynamic_state(VkPipelineDynamicStateCreateInfo* dynamicStateInfo, CCuint32 dynamicStatesCount, const VkDynamicState* dynamicStates) {
	dynamicStateInfo->sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicStateInfo->dynamicStateCount = dynamicStatesCount;
	dynamicStateInfo->pDynamicStates = dynamicStates;
}

CALLISTO_CORE_RESULT CC_VK_create_graphics_pipeline_layout(VkDevice* logicalDevice, VkPipelineLayout* pipelineLayout, CCuint32 descSetLayoutsSize, const VkDescriptorSetLayout* descSetLayouts, CCuint32 pushConstantRangeCount, const VkPushConstantRange* pPushConstantRanges) {
	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = descSetLayoutsSize;
	pipelineLayoutInfo.pSetLayouts = descSetLayouts;
	pipelineLayoutInfo.pushConstantRangeCount = pushConstantRangeCount;
	pipelineLayoutInfo.pPushConstantRanges = pPushConstantRanges;

	if (vkCreatePipelineLayout(*logicalDevice, &pipelineLayoutInfo, nullptr, pipelineLayout) == VK_SUCCESS)
		return CC_VK_GRAPHICS_PIPELINE_LAYOUT_CREATION_SUCCESS;
	else
		return CC_VK_GRAPHICS_PIPELINE_LAYOUT_CREATION_FAILURE;
}

CALLISTO_CORE_RESULT CC_VK_create_graphics_pipeline(VkDevice* logicalDevice, VkRenderPass* renderpass, VkPipeline* graphicsPipeline, CCuint32 shaderStagesCount, const VkPipelineShaderStageCreateInfo* shaderStages, VkPipelineVertexInputStateCreateInfo* vertexInputStateInfo, VkPipelineInputAssemblyStateCreateInfo* inputAssemblyStateInfo, VkPipelineViewportStateCreateInfo* viewportStateInfo, VkPipelineRasterizationStateCreateInfo* rasterizerStateInfo, VkPipelineMultisampleStateCreateInfo* multisamplingStateInfo, VkPipelineColorBlendStateCreateInfo* colorblendStateInfo, VkPipelineDynamicStateCreateInfo* dynamicStateInfo, VkPipelineLayout* pipelineLayout) {
	VkGraphicsPipelineCreateInfo pipelineInfo{};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = shaderStagesCount;
	pipelineInfo.pStages = shaderStages;
	pipelineInfo.pVertexInputState = vertexInputStateInfo;
	pipelineInfo.pInputAssemblyState = inputAssemblyStateInfo;
	pipelineInfo.pViewportState = viewportStateInfo;
	pipelineInfo.pRasterizationState = rasterizerStateInfo;
	pipelineInfo.pMultisampleState = multisamplingStateInfo;
	pipelineInfo.pColorBlendState = colorblendStateInfo;
	pipelineInfo.pDynamicState = dynamicStateInfo;
	pipelineInfo.layout = *pipelineLayout;
	pipelineInfo.renderPass = *renderpass;
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

	if (vkCreateGraphicsPipelines(*logicalDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, graphicsPipeline) == VK_SUCCESS)
		
		return CC_VK_GRAPHICS_PIPELINE_CREATION_SUCCESS;
	else
		return CC_VK_GRAPHICS_PIPELINE_CREATION_FAILURE;
}

void CC_VK_destroy_shader_module(VkDevice* logicalDevice, VkShaderModule* shaderModule) {
	vkDestroyShaderModule(*logicalDevice, *shaderModule, nullptr);
}

void CC_VK_destroy_graphics_pipeline_layout(VkDevice* logicalDevice, VkPipelineLayout* pipelineLayout) {
	vkDestroyPipelineLayout(*logicalDevice, *pipelineLayout, nullptr);
}

void CC_VK_destroy_graphics_pipeline(VkDevice* logicalDevice, VkPipeline* graphicsPipeline) {
	vkDestroyPipeline(*logicalDevice, *graphicsPipeline, nullptr);
}

CALLISTO_CORE_RESULT CC_VK_create_framebuffers(VkDevice* logicalDevice, std::vector<VkFramebuffer>& swapchainFramebuffers, const std::vector<VkImageView>& swapchainImageViews, VkRenderPass* renderpass, VkExtent2D* swapchainExtent) {
	swapchainFramebuffers.resize(swapchainImageViews.size());
	for (int i = 0; i < swapchainFramebuffers.size(); i++) {
		VkFramebufferCreateInfo framebufferCreateInfo{};
		framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferCreateInfo.renderPass = *renderpass;
		framebufferCreateInfo.attachmentCount = 1;
		framebufferCreateInfo.pAttachments = &swapchainImageViews[i];
		framebufferCreateInfo.width = swapchainExtent->width;
		framebufferCreateInfo.height = swapchainExtent->height;
		framebufferCreateInfo.layers = 1;

		if (vkCreateFramebuffer(*logicalDevice, &framebufferCreateInfo, nullptr, &swapchainFramebuffers[i]) != VK_SUCCESS)
			return CC_VK_FRAMEBUFFERS_CREATION_FAILURE;
	}
	return CC_VK_FRAMEBUFFERS_CREATION_SUCCESS;
}

void CC_VK_destroy_framebuffers(VkDevice* logicalDevice, std::vector<VkFramebuffer>& swapchainFramebuffers) {
	for (int i = 0; i < swapchainFramebuffers.size(); i++)
		vkDestroyFramebuffer(*logicalDevice, swapchainFramebuffers[i], nullptr);
}

CALLISTO_CORE_RESULT CC_VK_create_command_pool(VkDevice* logicalDevice, VkCommandPool* commandPool, VkPhysicalDevice* physicalDevice, VkSurfaceKHR* surface, VkCommandPoolCreateFlags flags) {
	p_CC_VK_PhysicalDeviceQueueFamilies devQueFam{};
	p_CC_VK_device_family_checker(*physicalDevice, *surface, &devQueFam);
	
	VkCommandPoolCreateInfo commandPoolInfo{};
	commandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	commandPoolInfo.flags = flags;
	try {
		commandPoolInfo.queueFamilyIndex = devQueFam.graphicsFamily.value();
	}
	catch (std::bad_optional_access) {
		throw std::runtime_error("HELLO!<");
	}

	if (vkCreateCommandPool(*logicalDevice, &commandPoolInfo, nullptr, commandPool) == VK_SUCCESS)
		return CC_VK_COMMAND_POOL_CREATION_SUCCESS;
	else
		return CC_VK_COMMAND_POOL_CREATION_FAILURE;
}

CALLISTO_CORE_RESULT CC_VK_create_command_buffers(VkDevice* logicalDevice, VkCommandPool* commandPool, VkCommandBuffer* commandBuffer, VkCommandBufferLevel level) {
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = *commandPool;
	allocInfo.level = level;
	allocInfo.commandBufferCount = 1;

	if (vkAllocateCommandBuffers(*logicalDevice, &allocInfo, commandBuffer) == VK_SUCCESS)
		return CC_VK_COMMAND_BUFFER_CREATION_SUCCESS;
	else
		return CC_VK_COMMAND_BUFFER_CREATION_FAILURE;
}

void CC_VK_destroy_command_pool(VkDevice* logicalDevice, VkCommandPool* commandPool) {
	vkDestroyCommandPool(*logicalDevice, *commandPool, nullptr);
}

CALLISTO_CORE_RESULT CC_VK_create_semaphore(VkDevice* logicalDevice, VkSemaphore* semaphore, VkSemaphoreCreateFlags flags) {
	VkSemaphoreCreateInfo semaphoreCreateInfo{};
	semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	semaphoreCreateInfo.flags = flags;

	if (vkCreateSemaphore(*logicalDevice, &semaphoreCreateInfo, nullptr, semaphore) == VK_SUCCESS)
		return CC_VK_SEMAPHORE_CREATION_SUCCESS;
	else
		return CC_VK_SEMAPHORE_CREATION_FAILURE;
}

CALLISTO_CORE_RESULT CC_VK_create_fence(VkDevice* logicalDevice, VkFence* fence, VkFenceCreateFlags flags) {
	VkFenceCreateInfo fenceCreateInfo{};
	fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceCreateInfo.flags = flags;

	if (vkCreateFence(*logicalDevice, &fenceCreateInfo, nullptr, fence) == VK_SUCCESS)
		return CC_VK_FENCE_CREATION_SUCCESS;
	else
		return CC_VK_FENCE_CREATION_FAILURE;
}

void CC_VK_wait_for_fences(VkDevice* logicalDevice, CCuint32 fenceCount, const VkFence* fences, VkBool32 waitForAll, CCuint64 timeout) {
	vkWaitForFences(*logicalDevice, fenceCount, fences, waitForAll, timeout);
}

void CC_VK_reset_fences(VkDevice* logicalDevice, CCuint32 fenceCount, const VkFence* fences) {
	vkResetFences(*logicalDevice, fenceCount, fences);
}

void CC_VK_destroy_semaphore(VkDevice* logicalDevice, VkSemaphore* semaphore) {
	vkDestroySemaphore(*logicalDevice, *semaphore, nullptr);
}

void CC_VK_destroy_fence(VkDevice* logicalDevice, VkFence* fence) {
	vkDestroyFence(*logicalDevice, *fence, nullptr);
}

CALLISTO_CORE_RESULT CC_VK_create_buffer(VkDevice* logicalDevice, VkBuffer* buffer, VkBufferCreateInfo* bufInfo, VkDeviceSize size, VkBufferUsageFlags usage, VkSharingMode sharingMode) {
	bufInfo->sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufInfo->size = size;
	bufInfo->usage = usage;
	bufInfo->sharingMode = sharingMode;

	if (vkCreateBuffer(*logicalDevice, bufInfo, nullptr, buffer) == VK_SUCCESS)
		return CC_VK_BUFFER_CREATION_SUCCESS;
	else
		return CC_VK_BUFFER_CREATION_FAILURE;
}

void CC_VK_get_buffer_memory_requirements(VkDevice* logicalDevice, VkBuffer* buffer, VkMemoryRequirements* memRequirements) {
	vkGetBufferMemoryRequirements(*logicalDevice, *buffer, memRequirements);
}

void CC_VK_get_image_memory_requirements(VkDevice* logicalDevice, VkImage* image, VkMemoryRequirements* memRequirements) {
	vkGetImageMemoryRequirements(*logicalDevice, *image, memRequirements);
}

CALLISTO_CORE_RESULT CC_VK_alloc_memory(VkDevice* logicalDevice, VkPhysicalDevice* physicalDevice, VkDeviceMemory* devMem, VkMemoryRequirements* memReq, VkMemoryPropertyFlags flags) {
	VkMemoryAllocateInfo memAllocInfo{};
	memAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memAllocInfo.allocationSize = memReq->size;
	memAllocInfo.memoryTypeIndex = p_CC_VK_find_mem_type_index(physicalDevice, memReq->memoryTypeBits, flags);

	if (vkAllocateMemory(*logicalDevice, &memAllocInfo, nullptr, devMem) == VK_SUCCESS)
		return CC_VK_MEMORY_ALLOCATED_SUCCESS;
	else
		return CC_VK_MEMORY_ALLOCATED_FAILURE;
}

void CC_VK_bind_buffer_memory(VkDevice* logicalDevice, VkBuffer* buffer, VkDeviceMemory* devMem, CCuint32 memOffset) {
	vkBindBufferMemory(*logicalDevice, *buffer, *devMem, memOffset);
}

void CC_VK_bind_image_memory(VkDevice* logicalDevice, VkImage* image, VkDeviceMemory* devMem, CCuint32 memOffset) {
	vkBindImageMemory(*logicalDevice, *image, *devMem, memOffset);
}

// CC_VK_mem_cpy_unmap is in CallistoCore.ipp

CALLISTO_CORE_RESULT CC_VK_create_image_sampler(VkDevice* logicalDevice, VkSampler* sampler, VkFilter magFilter, VkFilter minFilter, VkSamplerAddressMode addressModeU, VkSamplerAddressMode addressModeV, VkSamplerAddressMode addressModeW, VkBool32 anisotropyEnable, VkPhysicalDeviceProperties  physDevProp, VkBorderColor borderColor, VkBool32 unnormalizedCoordinates, VkBool32 compareEnable, VkCompareOp compareOp, VkSamplerMipmapMode mipmapMode) {
	VkSamplerCreateInfo samplerInfo{};
	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.magFilter = magFilter;
	samplerInfo.minFilter = minFilter;
	samplerInfo.addressModeU = addressModeU;
	samplerInfo.addressModeV = addressModeV;
	samplerInfo.addressModeW = addressModeW;
	samplerInfo.anisotropyEnable = anisotropyEnable;
	samplerInfo.maxAnisotropy = physDevProp.limits.maxSamplerAnisotropy;
	samplerInfo.borderColor = borderColor;
	samplerInfo.unnormalizedCoordinates = unnormalizedCoordinates;
	samplerInfo.compareEnable = compareEnable;
	samplerInfo.compareOp = compareOp;
	samplerInfo.mipmapMode = mipmapMode;

	if (vkCreateSampler(*logicalDevice, &samplerInfo, nullptr, sampler) == VK_SUCCESS)
		return CC_VK_IMAGE_SAMPLER_CREATION_SUCCESS;
	else
		return CC_VK_IMAGE_SAMPLER_CREATION_FAILURE;
}

CALLISTO_CORE_RESULT CC_VK_create_image_view(VkDevice* logicalDevice, VkImageView* imageView, VkImage* image, VkImageViewType viewType, VkFormat format, VkImageAspectFlags aspectMap, CCuint32 baseMipLevel, CCuint32 levelCount, CCuint32 baseArrayLayer, CCuint32 layerCount) {
	VkImageViewCreateInfo viewInfo{};
	viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewInfo.image = *image;
	viewInfo.viewType = viewType;
	viewInfo.format = format;
	viewInfo.subresourceRange.aspectMask = aspectMap;
	viewInfo.subresourceRange.baseMipLevel = baseMipLevel;
	viewInfo.subresourceRange.levelCount = levelCount;
	viewInfo.subresourceRange.baseArrayLayer = baseArrayLayer;
	viewInfo.subresourceRange.layerCount = layerCount;

	if (vkCreateImageView(*logicalDevice, &viewInfo, nullptr, imageView) == VK_SUCCESS)
		return CC_VK_IMAGE_VIEW_CREATION_SUCCESS;
	else
		return CC_VK_IMAGE_VIEW_CREATION_FAILURE;
}

void CC_VK_destroy_image(VkDevice* logicalDevice, VkImage* image) {
	vkDestroyImage(*logicalDevice, *image, nullptr);
}

void CC_VK_destroy_image_sampler(VkDevice* logicalDevice, VkSampler* sampler) {
	vkDestroySampler(*logicalDevice, *sampler, nullptr);
}

void CC_VK_destroy_image_view(VkDevice* logicalDevice, VkImageView* imageView) {
	vkDestroyImageView(*logicalDevice, *imageView, nullptr);
}

void CC_VK_destroy_buffer(VkDevice* logicalDevice, VkBuffer* buffer) {
	vkDestroyBuffer(*logicalDevice, *buffer, nullptr);
}

void CC_VK_destroy_device_mem(VkDevice* logicalDevice, VkDeviceMemory* devMem) {
	vkFreeMemory(*logicalDevice, *devMem, nullptr);
}

void CC_VK_map_mem(VkDevice* logicalDevice, VkDeviceMemory* devMem, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void** data) {
	vkMapMemory(*logicalDevice, *devMem, offset, size, flags, data);
}

void CC_VK_unmap_mem(VkDevice* logicalDevice, VkDeviceMemory* devMem) {
	vkUnmapMemory(*logicalDevice, *devMem);
}

CALLISTO_CORE_RESULT CC_load_image_data(stbi_uc** imageData, CCwords filename, CCint* imageWidth, CCint* imageHeight, CCint* imageChannels, CCint colorFormat) {
	*imageData = stbi_load(filename, imageWidth, imageHeight, imageChannels, colorFormat);
	if (imageData == nullptr)
		return CC_IMAGE_DATA_LOADED_FAILED;
	else
		return CC_IMAGE_DATA_LOADED_SUCCESSFULLY;
}

CALLISTO_CORE_RESULT CC_VK_create_image(VkDevice* logicalDevice, VkImage* image, VkImageType imageType, CCuint32 width, CCuint32 height, CCuint32 depth, CCuint32 mipLevels, CCuint32 arrayLayers, VkFormat format, VkImageTiling tiling, VkImageLayout initialLayout, VkImageUsageFlags usage, VkSampleCountFlagBits samples, VkSharingMode sharingMode) {
	VkImageCreateInfo imgCreateInfo{};
	imgCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imgCreateInfo.imageType = imageType;
	imgCreateInfo.extent.width = width;
	imgCreateInfo.extent.height = height;
	imgCreateInfo.extent.depth = depth;
	imgCreateInfo.mipLevels = mipLevels;
	imgCreateInfo.arrayLayers = arrayLayers;
	imgCreateInfo.format = format;
	imgCreateInfo.tiling = tiling;
	imgCreateInfo.initialLayout = initialLayout;
	imgCreateInfo.usage = usage;
	imgCreateInfo.samples = samples;
	imgCreateInfo.sharingMode = sharingMode;

	if (vkCreateImage(*logicalDevice, &imgCreateInfo, nullptr, image) == VK_SUCCESS)
		return CC_VK_IMAGE_CREATION_SUCCESS;
	else
		return CC_VK_IMAGE_CREATION_FAILURE;
}

void CC_VK_copy_buffer_to_image(VkCommandBuffer* commandBuffer, VkBuffer* buffer, VkImage* image, VkDeviceSize bufferOffset, CCuint32 bufferRowLength, CCuint32 bufferImageHeight, VkImageAspectFlags aspectMask, CCuint32 mipLevel, CCuint32 baseArrayLayer, CCuint32 layerCount, CCint imageOffsetX, CCint imageOffsetY, CCint imageOffsetZ, CCuint32 width, CCuint32 height, CCuint32 depth) {
	VkBufferImageCopy region{};
	region.bufferOffset = bufferOffset;
	region.bufferRowLength = bufferRowLength;
	region.bufferImageHeight = bufferImageHeight;
	region.imageSubresource.aspectMask = aspectMask;
	region.imageSubresource.mipLevel = mipLevel;
	region.imageSubresource.baseArrayLayer = baseArrayLayer;
	region.imageSubresource.layerCount = 1;
	region.imageOffset = { imageOffsetX, imageOffsetY, imageOffsetZ };
	region.imageExtent = { width, height, depth };

	vkCmdCopyBufferToImage(*commandBuffer, *buffer, *image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
}

void CC_VK_pipeline_barrier(VkCommandBuffer* commandBuffer, VkImageLayout oldLayout, VkImageLayout newLayout, CCuint32 srcQueueFamilyIndex, CCuint32 dstQueueFamilyIndex, VkImage* image, VkImageAspectFlags aspectMask, CCuint32 baseMipLevel, CCuint32 levelCount, CCuint32 baseArrayLayer, CCuint32 layerCount, VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask, VkPipelineStageFlags sourceStage, VkPipelineStageFlags destinationStage) {
	VkImageMemoryBarrier barrier{};																																													  
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;																																							
	barrier.oldLayout = oldLayout;																																													  
	barrier.newLayout = newLayout;
	barrier.srcQueueFamilyIndex = srcQueueFamilyIndex;
	barrier.dstQueueFamilyIndex = dstQueueFamilyIndex;
	barrier.image = *image;
	barrier.subresourceRange.aspectMask = aspectMask;
	barrier.subresourceRange.baseMipLevel = baseMipLevel;
	barrier.subresourceRange.levelCount = levelCount;
	barrier.subresourceRange.baseArrayLayer = baseArrayLayer;
	barrier.subresourceRange.layerCount = layerCount;
	barrier.srcAccessMask = srcAccessMask;
	barrier.dstAccessMask = dstAccessMask;
	
	vkCmdPipelineBarrier(*commandBuffer, sourceStage, destinationStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);
}

void CC_free_image_data(stbi_uc** imageData) {
	stbi_image_free(*imageData);
}

CALLISTO_CORE_RESULT CC_VK_begin_command_buffers(VkCommandBuffer* commandBuffer, VkCommandBufferUsageFlags flags) {
	VkCommandBufferBeginInfo cmdBuffersCreateInfo{};
	cmdBuffersCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	cmdBuffersCreateInfo.flags = flags;
	
	if (vkBeginCommandBuffer(*commandBuffer, &cmdBuffersCreateInfo) == VK_SUCCESS)
		return CC_VK_COMMAND_BUFFER_STARTED_SUCCESFULLY;
	else
		return CC_VK_COMMAND_BUFFER_STARTED_FAILED;
}

void CC_VK_get_image_index_in_framebuffers(VkDevice* logicalDevice, VkSwapchainKHR* swapchain, CCuint64 timeout, VkSemaphore* semaphore, VkFence* fence, CCuint32* imageIndex) {
	if (fence == nullptr)
		vkAcquireNextImageKHR(*logicalDevice, *swapchain, timeout, *semaphore, VK_NULL_HANDLE, imageIndex);
	else
		vkAcquireNextImageKHR(*logicalDevice, *swapchain, timeout, *semaphore, *fence, imageIndex);
}

void CC_VK_begin_renderpass(VkCommandBuffer* commandBuffer, VkRenderPass* renderpass, const std::vector<VkFramebuffer>& swapchainFramebuffers, CCuint32 imageIndex, VkExtent2D* swapchainExtent) {
	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = *renderpass;
	renderPassInfo.framebuffer = swapchainFramebuffers[imageIndex];
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = *swapchainExtent;

	VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
	renderPassInfo.clearValueCount = 1;
	renderPassInfo.pClearValues = &clearColor;

	vkCmdBeginRenderPass(*commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void CC_VK_bind_pipeline(VkCommandBuffer* commandBuffer, VkPipelineBindPoint bindPoint, VkPipeline* pipeline) {
	vkCmdBindPipeline(*commandBuffer, bindPoint, *pipeline);
}

void CC_VK_viewport(VkCommandBuffer* commandBuffer, CCfloat x, CCfloat y, CCfloat width, CCfloat height, CCfloat minDepth, CCfloat maxDepth) {
	VkViewport viewportInfo{};
	viewportInfo.x = x;
	viewportInfo.y = y;
	viewportInfo.width = width;
	viewportInfo.height = height;
	viewportInfo.minDepth = minDepth;
	viewportInfo.maxDepth = maxDepth;
	vkCmdSetViewport(*commandBuffer, 0, 1, &viewportInfo);
}

void CC_VK_scissor(VkCommandBuffer* commandBuffer, CCint topLeftX, CCint topLeftY, VkExtent2D* swapchainExtent) {
	VkRect2D scissorInfo{};
	scissorInfo.offset = {topLeftX, topLeftY};
	scissorInfo.extent = *swapchainExtent;
	vkCmdSetScissor(*commandBuffer, 0, 1, &scissorInfo);
}

void CC_VK_bind_vertex_buffers(VkCommandBuffer* commandBuffer, CCuint32 firstBinding, CCuint32 bindingCount, const VkBuffer* buffer, const VkDeviceSize* offsets) {
	vkCmdBindVertexBuffers(*commandBuffer, firstBinding, bindingCount, buffer, offsets);
}

void CC_VK_bind_index_buffers(VkCommandBuffer* commandBuffer, VkBuffer* indexBuffers, VkDeviceSize offset, VkIndexType indexType) {
	vkCmdBindIndexBuffer(*commandBuffer, *indexBuffers, offset, indexType);
}

void CC_VK_bind_descriptor_sets(VkCommandBuffer* commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout pipelineLayout, CCuint32 firstSet, CCuint32 descSetsCount, const VkDescriptorSet* descSets, CCuint32 offsetCount, const CCuint32* offsets) {
	vkCmdBindDescriptorSets(*commandBuffer, pipelineBindPoint, pipelineLayout, firstSet, descSetsCount, descSets, offsetCount, offsets);
}

void CC_VK_draw(VkCommandBuffer* commandBuffer, CCuint32 vertexCount, CCuint32 instanceCount, CCuint32 firstVertex, CCuint32 firstInstance) {
	vkCmdDraw(*commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
}

void CC_VK_indexed_draw(VkCommandBuffer* commandBuffer, CCuint32 indexCount, CCuint32 instanceCount, CCuint32 firstIndex, CCuint32 vertexOffset, CCuint32 firstInstance) {
	vkCmdDrawIndexed(*commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
}

void CC_VK_end_renderpass(VkCommandBuffer* commandBuffer) {
	vkCmdEndRenderPass(*commandBuffer);
}

CALLISTO_CORE_RESULT CC_VK_end_command_buffers(VkCommandBuffer* commandBuffer) {
	if (vkEndCommandBuffer(*commandBuffer) == VK_SUCCESS)
		return CC_VK_COMMAND_BUFFER_ENDED_SUCCESFULLY;
	else
		return 	CC_VK_COMMAND_BUFFER_ENDED_FAILED;
}

void CC_VK_reset_command_buffers(VkCommandBuffer* commandBuffer, VkCommandBufferResetFlags flags) {
	vkResetCommandBuffer(*commandBuffer, flags);
}

CALLISTO_CORE_RESULT CC_VK_submit_queue(VkCommandBuffer* commandBuffer, VkQueue* graphicsQueue) {
	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = commandBuffer;

	if (vkQueueSubmit(*graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE) == VK_SUCCESS)
		return CC_VK_QUEUE_SUBMITTED_SUCCESSFULLY;
	else
		return CC_VK_QUEUE_SUBMITTED_FAILED;
}

CALLISTO_CORE_RESULT CC_VK_submit_queue(VkCommandBuffer* commandBuffer, CCuint32 commandBufferCount, VkSemaphore* waitSemaphore, CCuint32 waitSemaphoreCount, VkSemaphore* signalSemaphore, CCuint32 signalSemaphoreCount, const VkPipelineStageFlags* pipelineStageFlags, VkFence* inFlightFence, VkQueue* graphicsQueue) {
	VkSemaphore waitSemaphores[] = { *waitSemaphore };
	VkSemaphore signalSemaphores[] = { *signalSemaphore };
	
	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.waitSemaphoreCount = waitSemaphoreCount;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = pipelineStageFlags;
	submitInfo.commandBufferCount = commandBufferCount;
	submitInfo.pCommandBuffers = commandBuffer;
	submitInfo.signalSemaphoreCount = signalSemaphoreCount;
	submitInfo.pSignalSemaphores = signalSemaphores;

	if (vkQueueSubmit(*graphicsQueue, 1, &submitInfo, *inFlightFence) == VK_SUCCESS)
		return CC_VK_QUEUE_SUBMITTED_SUCCESSFULLY;
	else
		return CC_VK_QUEUE_SUBMITTED_FAILED;
}

CALLISTO_CORE_RESULT CC_VK_present_queue(VkSwapchainKHR* swapchain, VkQueue* presentQueue, VkSemaphore* signalSemaphore, CCuint32 imageIndex) {
	VkSwapchainKHR swapChains[] = { *swapchain };
	
	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphore;
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;

	if (vkQueuePresentKHR(*presentQueue, &presentInfo) == VK_SUCCESS)
		return CC_VK_QUEUE_PRESENTED_SUCCESSFULLY;
	else
		return CC_VK_QUEUE_PRESENTED_FAILED;
}

void CC_VK_wait_device_idle(VkDevice* logicalDevice) {
	vkDeviceWaitIdle(*logicalDevice);
}

void CC_VK_wait_queue_idle(VkQueue* queue) {
	vkQueueWaitIdle(*queue);
}

//<<----- Vulkan ------>//

//<<----- Public Functions ------>//