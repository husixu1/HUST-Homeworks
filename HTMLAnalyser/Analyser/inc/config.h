#pragma once

#define SYSTEM_WIN32
#define STATUS_TEST
#define STATUS_DEBUG_NO

#ifdef STATUS_DEBUG
#	define DEBUG_ALERT(x) std::cout << x << std::endl;
#else
#	define DEBUG_ALERT(x) ;
#endif

//#define SYSTEM_LINUX
