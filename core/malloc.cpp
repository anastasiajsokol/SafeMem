#include "malloc.h"

#include "platformdescriptor.h"
#include <cstdint>

#include <stdio.h>

namespace safemem {
    constexpr std::size_t buffer_length = 1024;

    static struct {
        std::uint8_t data[buffer_length];
        bool used = false;
    } buffer;
};

extern "C" void* aligned_alloc(std::size_t alignment, std::size_t size){
    if(safemem::PlatformDescriptor::load()){
        return safemem::PlatformDescriptor::aligned_alloc(alignment, size);
    } else if(size <= safemem::buffer_length && safemem::buffer.used == false){
        return safemem::buffer.data;
    }

    return nullptr;
}

extern "C" void* realloc(void* memory, std::size_t size){
    if(memory == safemem::buffer.data){
        return nullptr;
    }

    if(safemem::PlatformDescriptor::load()){
        return safemem::PlatformDescriptor::realloc(memory, size);
    }

    return nullptr;
}

extern "C" void* calloc(std::size_t number, std::size_t size){
    if(safemem::PlatformDescriptor::load()){
        return safemem::PlatformDescriptor::calloc(number, size);
    } else if(number * size <= safemem::buffer_length && !safemem::buffer.used){
        safemem::buffer.used = true;
        return safemem::buffer.data;
    }

    return nullptr;
}

extern "C" void* malloc(std::size_t size){
    if(safemem::PlatformDescriptor::load()){
        return safemem::PlatformDescriptor::malloc(size);
    } else if(size <= safemem::buffer_length && !safemem::buffer.used){
        safemem::buffer.used = true;
        return safemem::buffer.data;
    }

    return nullptr;
}

extern "C" void free(void* memory){
    if(memory == safemem::buffer.data){
        safemem::buffer.used = false;
        return;
    }

    if(safemem::PlatformDescriptor::load()){
        safemem::PlatformDescriptor::free(memory);
    }
}