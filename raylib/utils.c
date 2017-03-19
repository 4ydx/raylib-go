/**********************************************************************************************
*
*   raylib.utils - Some common utility functions
*
*   CONFIGURATION:
*
*   #define SUPPORT_SAVE_PNG
*       Enable saving PNG fileformat
*       NOTE: Requires stb_image_write library
*
*   #define SUPPORT_SAVE_BMP
*
*   #define DO_NOT_TRACE_DEBUG_MSGS
*       Avoid showing DEBUG TraceLog() messages
*
*   DEPENDENCIES:
*       stb_image_write - PNG writting functions
*
*
*   LICENSE: zlib/libpng
*
*   Copyright (c) 2014-2016 Ramon Santamaria (@raysan5)
*
*   This software is provided "as-is", without any express or implied warranty. In no event
*   will the authors be held liable for any damages arising from the use of this software.
*
*   Permission is granted to anyone to use this software for any purpose, including commercial
*   applications, and to alter it and redistribute it freely, subject to the following restrictions:
*
*     1. The origin of this software must not be misrepresented; you must not claim that you
*     wrote the original software. If you use this software in a product, an acknowledgment
*     in the product documentation would be appreciated but is not required.
*
*     2. Altered source versions must be plainly marked as such, and must not be misrepresented
*     as being the original software.
*
*     3. This notice may not be removed or altered from any source distribution.
*
**********************************************************************************************/

#include "utils.h"

#if defined(PLATFORM_ANDROID)
    #include <errno.h>
    #include <android/log.h>
    #include <android/asset_manager.h>
#endif

#include <stdlib.h>                 // Required for: malloc(), free()
#include <stdio.h>                  // Required for: fopen(), fclose(), fputc(), fwrite(), printf(), fprintf(), funopen()
#include <stdarg.h>                 // Required for: va_list, va_start(), vfprintf(), va_end()
#include <string.h>                 // Required for: strlen(), strrchr(), strcmp()

#if defined(PLATFORM_DESKTOP) || defined(PLATFORM_RPI)
    #define STB_IMAGE_WRITE_IMPLEMENTATION
    #include "external/stb_image_write.h"    // Required for: stbi_write_bmp(), stbi_write_png()
#endif

#define RRES_IMPLEMENTATION
#include "rres.h"

//#define NO_TRACELOG                 // Avoid TraceLog() output (any type)
#define DO_NOT_TRACE_DEBUG_MSGS     // Avoid DEBUG messages tracing

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
#if defined(PLATFORM_ANDROID)
AAssetManager *assetManager;
#endif

//----------------------------------------------------------------------------------
// Module specific Functions Declaration
//----------------------------------------------------------------------------------
#if defined(PLATFORM_ANDROID)
static int android_read(void *cookie, char *buf, int size);
static int android_write(void *cookie, const char *buf, int size);
static fpos_t android_seek(void *cookie, fpos_t offset, int whence);
static int android_close(void *cookie);
#endif

//----------------------------------------------------------------------------------
// Module Functions Definition - Utilities
//----------------------------------------------------------------------------------
// Outputs a trace log message
void TraceLog(int msgType, const char *text, ...)
{
#if !defined(NO_TRACELOG)
    static char buffer[128];
    int traceDebugMsgs = 1;
    
#ifdef DO_NOT_TRACE_DEBUG_MSGS
    traceDebugMsgs = 0;
#endif

    switch(msgType)
    {
        case INFO: strcpy(buffer, "INFO: "); break;
        case ERROR: strcpy(buffer, "ERROR: "); break;
        case WARNING: strcpy(buffer, "WARNING: "); break;
        case DEBUG: strcpy(buffer, "DEBUG: "); break;
        default: break;
    }

    strcat(buffer, text);
    strcat(buffer, "\n");

    va_list args;
    va_start(args, text);

#if defined(PLATFORM_ANDROID)
    switch(msgType)
    {
        case INFO: __android_log_vprint(ANDROID_LOG_INFO, "raylib", buffer, args); break;
        case ERROR: __android_log_vprint(ANDROID_LOG_ERROR, "raylib", buffer, args); break;
        case WARNING: __android_log_vprint(ANDROID_LOG_WARN, "raylib", buffer, args); break;
        case DEBUG: if (traceDebugMsgs) __android_log_vprint(ANDROID_LOG_DEBUG, "raylib", buffer, args); break;
        default: break;
    }
#else
    if ((msgType != DEBUG) || ((msgType == DEBUG) && (traceDebugMsgs))) vprintf(buffer, args);
#endif

    va_end(args);

    if (msgType == ERROR) exit(1);  // If ERROR message, exit program
    
#endif  // NO_TRACELOG
}

#if defined(PLATFORM_DESKTOP) || defined(PLATFORM_RPI)
// Creates a BMP image file from an array of pixel data
void SaveBMP(const char *fileName, unsigned char *imgData, int width, int height, int compSize)
{
    stbi_write_bmp(fileName, width, height, compSize, imgData);
}

// Creates a PNG image file from an array of pixel data
void SavePNG(const char *fileName, unsigned char *imgData, int width, int height, int compSize)
{
    stbi_write_png(fileName, width, height, compSize, imgData, width*compSize);
}
#endif

#if defined(PLATFORM_ANDROID)
// Initialize asset manager from android app
void InitAssetManager(AAssetManager *manager)
{
    assetManager = manager;
}

// Replacement for fopen
FILE *android_fopen(const char *fileName, const char *mode)
{
    if (mode[0] == 'w') return NULL;

    AAsset *asset = AAssetManager_open(assetManager, fileName, 0);

    if (!asset) return NULL;

    return funopen(asset, android_read, android_write, android_seek, android_close);
}
#endif

// Keep track of memory allocated
// NOTE: mallocType defines the type of data allocated
/*
void RecordMalloc(int mallocType, int mallocSize, const char *msg)
{
    // TODO: Investigate how to record memory allocation data...
    // Maybe creating my own malloc function...
}
*/

// Get the extension for a filename
const char *GetExtension(const char *fileName)
{
    const char *dot = strrchr(fileName, '.');
    if (!dot || dot == fileName) return "";
    return (dot + 1);
}

//----------------------------------------------------------------------------------
// Module specific Functions Definition
//----------------------------------------------------------------------------------
#if defined(PLATFORM_ANDROID)
static int android_read(void *cookie, char *buf, int size)
{
    return AAsset_read((AAsset *)cookie, buf, size);
}

static int android_write(void *cookie, const char *buf, int size)
{
    TraceLog(ERROR, "Can't provide write access to the APK");

    return EACCES;
}

static fpos_t android_seek(void *cookie, fpos_t offset, int whence)
{
    return AAsset_seek((AAsset *)cookie, offset, whence);
}

static int android_close(void *cookie)
{
    AAsset_close((AAsset *)cookie);
    return 0;
}
#endif
