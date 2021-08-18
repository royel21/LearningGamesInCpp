// glfw_config.h configuration header file.  If you are adding a feature
// requiring conditional compilation, this is where to add the macro.
//========================================================================
// As glfw_config.h, this file defines compile-time option macros for a
// specific platform and development environment.  If you are using the
// GLFW CMake files, modify glfw_config.h.in instead of this file.  If you
// are using your own build system, make this file define the appropriate
// macros in whatever way is suitable.
//========================================================================

// MODIFIED_ERIN
#ifdef _WIN32
#define _GLFW_WIN32
#define _CRT_SECURE_NO_WARNINGS
#elif __APPLE__
#define _GLFW_COCOA
#else
#define _GLFW_X11
#endif