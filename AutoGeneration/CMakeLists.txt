cmake_minimum_required(VERSION 3.12)

### IMPORTANT ##############################################
# Set the project name to your liking: (Include quotes)
# Example: "CallistoCoreProject"
set(PROJECT_NAME "YourProject")
############################################################

project(${PROJECT_NAME})

### IMPORTANT ##############################################
# Set the path to the Vulkan SDK (Include quotes)
# Example: "C:\\VulkanSDK\\1.3.261.1"
set(VULKAN_DIR "C:\\VulkanSDK\\1.3.261.1")
############################################################

### For Linking CC #########################################
# Remember this only links the debug. (This can be changed, depending on which one you want first)

include_directories("${CMAKE_SOURCE_DIR}\\CallistoCore\\incl")

# For debug:
link_directories("${CMAKE_SOURCE_DIR}\\CallistoCore\\libs\\x64-debug")

# For release:
# link_directories("${CMAKE_SOURCE_DIR}\\CallistoCore\\libs\\x64-release")
############################################################

# 3rd Party Includes
include_directories("${CMAKE_SOURCE_DIR}\\dependencies\\incl")
include_directories("${VULKAN_DIR}\\Include")

set(CMAKE_CXX_STANDARD 20)

# Create the executable
add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE "CallistoCore.lib")

set(CMAKE_SUPPRESS_REGENERATION true)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}\\${PROJECT_NAME}")