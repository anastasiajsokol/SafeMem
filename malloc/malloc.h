/**
 *   Dynamically replace malloc with wrapper that checks usage
 *      author      Anastasia Sokol
 *      liscense    Unliscense <https://unlicense.org>
 *      requires    c++17 to c++20, dlfcn.h or windows.h
 *      
 *      functions should work as specified in cstdlib standard
 *      I am not sure what happens if combined with GCC malloc hooks or similar systems
 *      TODO: add support for c++23 free_size and free_aligned_size
**/

#include <cstdint>

namespace safemem {
    enum class MemoryStatus {
        ok = 0,
        warning = 0,
        error = 0
    };

    // Stores basic information about the memory usage of the program
    struct GlobalMemoryUsage {
        inline static MemoryStatus status = MemoryStatus::ok;
        inline static std::size_t usage = 0;
    };
};

extern "C" void* aligned_alloc(std::size_t alignment, std::size_t size);
extern "C" void* calloc(std::size_t number, std::size_t size);
extern "C" void* realloc(void* memory, std::size_t size);
extern "C" void* malloc(std::size_t size);
extern "C" void  free(void* memory);
