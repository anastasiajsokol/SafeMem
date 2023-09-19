#include "malloc.h"

#include "platformdescriptor.h"
#include <cstdint>

void* aligned_alloc(std::size_t alignment, std::size_t size){
    if(safemem::PlatformDescriptor::safe()){
        return safemem::PlatformDescriptor().aligned_alloc(alignment, size);
    }

    return nullptr;
}

void* realloc(void* memory, std::size_t size){
    if(safemem::PlatformDescriptor::safe()){
        return safemem::PlatformDescriptor().realloc(memory, size);
    }

    return nullptr;
}

void* calloc(std::size_t number, std::size_t size){
    if(safemem::PlatformDescriptor::safe()){
        return safemem::PlatformDescriptor().calloc(number, size);
    }

    return nullptr;
}

void* malloc(std::size_t size){
    if(safemem::PlatformDescriptor::safe()){
        return safemem::PlatformDescriptor().malloc(size);
    }

    return nullptr;
}

void free(void* memory){
    if(safemem::PlatformDescriptor::safe()){
        safemem::PlatformDescriptor().free(memory);
    }
}