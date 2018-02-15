#pragma once

// Zero out platform definitions
#if !defined(OS_WINDOWS)
	#define OS_WINDOWS 0
#endif
#if !defined(OS_MACINTOSH)
	#define OS_MACINTOSH 0
#endif
#if !defined(OS_LINUX)
	#define OS_LINUX 0
#endif
#if !defined(OS_BSD)
	#define OS_BSD 0
#endif
#if !defined(OS_ANDROID)
	#define OS_ANDROID 0
#endif
#if !defined(OS_IOS)
	#define OS_IOS 0
#endif
#if !defined(PLATFORM_BIGENDIAN)
	#define PLATFORM_BIGENDIAN 0
#endif

// Ignore Redefinition
#pragma warning (push)
#pragma warning (disable : 4005)

// Determine the platform
#if   defined(__ANDROID__)
	#define OS_ANDROID 1
#elif defined(__bsdi__) || defined(BSD)
	#define OS_BSD 1
#elif defined(__linux__) || defined(linux) || defined(__linux) || defined(__gnu_linux__)
	#define OS_LINUX 1
#elif defined(macintosh) || defined(Macintosh) || (defined(__APPLE__) && defined(__MACH__))
	#define OS_MACINTOSH 1
#elif defined(_WIN16) || defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(__WINDOWS__)
	#define OS_WINDOWS 1
#elif defined(__APPLE__) && !defined(__MACH__)
	#define OS_IOS 1
#endif

#if (OS_WINDOWS || OS_MACINTOSH || OS_IOS)
	#define PLATFORM_BIGENDIAN 0
#else
#include <endian.h>
#if (__BYTE_ORDER == __LITTLE_ENDIAN)
	#define PLATFORM_BIGENDIAN 0
#else
	#define PLATFORM_BIGENDIAN 1
#endif
#endif

#pragma warning (pop)