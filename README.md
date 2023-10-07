
# Callisto Core
Callisto Core is a Vulkan-based framework for rendering graphics. It abstracts Vulkan a little bit while allowing most of the flexibility to Vulkan.

There is only support for **Windows**.

##  How to build the project

- **Pre-compiled libraries** are available so this is not needed, if you don't want to build it yourself, just skip it this whole part.
- To build the project, open the folder into MSVC. 
	- I assume the latest MSVC (17 2022)
- Once opened in MSVC, the **out** folder will get created.
- With commands run the **build.ninja** file in the **out** folder.
	- It doesn't matter which folder you use, I just use the debug version since it will build the same MSVC solution:
	- **Commands** (in a .bat file):
	- *cd out\build\x64-debug
		del CMakeCache.txt
		ninja
		PAUSE*
- The MSVC solution will then get created and then the **.lib** files can be compiled into x64 debug and release modes.
- And also the CMake file does not link the libraries required, only the include files.
- If this is not done, then these libraries would have to be linked in your project.
- Not linking won't stop Callisto Core from compiling.
	- To link do the following: 
		- Make sure you are in **All** Configurations
		- Go to Linker **->** General
		- In the **Additional Dependencies** add: **glfw3.lib; vulkan-1.lib; shaderc_shared.lib;**
		- In the **Additional Library Directories** add: **$(VULKAN_SDK)\Lib; \.\.\\\.\.\\\.\.\\dependencies\libs\GLFW;**

## How to use the framework

I have provided a folder that holds the creation tools.
Included in the folder is a folder called **Runtimes**. When distributing your executables, make sure to place copy the **Runtimes** folder into the folder you wish to distribute.

- In the **CMakeLists.txt** in the project creation folder, you will need to edit some values.
	- Set the title for your project in **set(PROJECT_NAME "Your Project Name")**
	- Be sure to set the path to the Vulkan SDK in **set(VULKAN_DIR "path/to/vulkan")**
	- **Note:**
		- The CMake file only links the debug version of Callisto Core.
		- To link the release version, you will have to manually do it.
		- To link do the following:
			- Go to the properties of your solution in MSVC.
			- Make sure you are in the **Release** Configuration
			- Go to Linker **->** General **->** Additional Library Directories
			- Edit  **\.\.\\CallistoCore\\libs\\x64-debug** to **\.\.\\CallistoCore\\libs\\x64-release**
