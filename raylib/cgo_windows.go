// +build windows

package raylib

/*
#include "external/glfw/src/context.c"
#include "external/glfw/src/init.c"
#include "external/glfw/src/input.c"
#include "external/glfw/src/monitor.c"
#include "external/glfw/src/vulkan.c"
#include "external/glfw/src/window.c"

#include "external/glfw/src/win32_init.c"
#include "external/glfw/src/win32_joystick.c"
#include "external/glfw/src/win32_monitor.c"
#include "external/glfw/src/win32_time.c"
#include "external/glfw/src/win32_tls.c"
#include "external/glfw/src/win32_window.c"
#include "external/glfw/src/wgl_context.c"
#include "external/glfw/src/egl_context.c"

#cgo windows,!static LDFLAGS: -lopengl32 -lgdi32 -lopenal -lwinmm -lole32
#cgo windows,!static CFLAGS: -D_GLFW_WIN32 -Iexternal/glfw/include -Iexternal/glfw/deps/mingw -DPLATFORM_DESKTOP -DGRAPHICS_API_OPENGL_33 -DSHARED_OPENAL

#cgo windows,static LDFLAGS: -lopengl32 -lgdi32 -lopenal -lwinmm -lole32
#cgo windows,static CFLAGS: -D_GLFW_WIN32 -Iexternal/glfw/include -Iexternal/glfw/deps/mingw -DPLATFORM_DESKTOP -DGRAPHICS_API_OPENGL_33 -DAL_LIBTYPE_STATIC
*/
import "C"
