/**
 *  Platform dependant code for dynamically fetching symbols
 *      Linux and MaxOS - dlfcn.h
 *      Windows         - windows.h
 *      Other platforms not supported
 * 
 *      Possible Issues
 *          On linux it is possible to link against ld-*.so.2 in such a way that the 'malloc' and 'free' are defined there not libc.so.6
**/

#include "dynamicload.h"

#if __linux__ || __unix__ || defined(_POSIX_VERSION) || __APPLE__

#include <dlfcn.h>

void* dynamic_fetch_symbol_address(const char* symbol){
    // there are lots of weird things that can happen here, however,
    // most of the issues are things that do not really matter for debugging
    // (for example, the free symbol this returns may not actually free)
    // and even if we detected this (which we could) there is not much that we could do
    void* address = dlsym(RTLD_NEXT, symbol);
}

#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

#include <windows.h>

void* dynamic_fetch_symbol_address(const char* symbol){
    // attempt to load from c standard library - ucrtbase.dll
    void* symbol = GetProcAddress(GetModuleHandle(TEXT("ucrtbase.dll")), symbol);

    if(symbol == nullptr){
        // if unable to load from ucrtbase.dll, try MSVC implementation up to v6.0 
        symbol = GetProcAddress(GetModuleHandle(TEXT("msvcrt.dll")), symbol);
    }

    return symbol;
}

#else
    #error You may not be using a supported platform
#endif