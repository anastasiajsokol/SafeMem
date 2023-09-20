/**
 *  Replace default global new and delete operators with a version that keeps track of memory usage
 *      author      Anastasia Sokol
 *      liscense    Unliscense <https://unlicense.org>
 *      requires    c++17
 *  
 *  Requirements of a standard new operator
 *      1) Return ownership to at least 'size' bytes
 *      2) Be aligned such that it can converted to a pointer of any complete object type
 *          exception: if std::align_val_t is passed data must be aligned its value
 *      3) Return a valid, unique pointer even if zero bytes are requested
 *      4) On a failed allocation call new handler then try again until the new handler is a nullptr
 *      5) If memory can not be allocated, throw std::bad_alloc
 *          exception: if std::nothrow_t is passed as an argument, return a nullptr on failure
 * 
 *  Requirements of a standard delete operator
 *      1) May not throw an exception
 *      2) Must safely handle being passed a nullptr
 *      3) If the pointer passed was allocated by new, delete should deallocate that memory
 *          note: interestingly this applies even if delete is scoped to a class and a pointer to something else is passed
 *  
 *  WARNING: While most platforms allow free(nullptr), Palm OS amoung others does not
**/

#include "safemem.h"

#include <cstdlib>  // defines malloc and free which do most of the real work
#include <new>      // defines std::bad_alloc, std::new_handler, std::align_val_t, and std::nothrow_t

inline static std::size_t usage = 0;

inline std::new_handler get_new_handler(){
    std::new_handler handler = std::set_new_handler(nullptr);
    std::set_new_handler(handler);
    return handler;
}

void* allocate(std::size_t size, std::align_val_t alignment = std::align_val_t(__STDCPP_DEFAULT_NEW_ALIGNMENT__)){
    // save space for storing size
    const size_t padded_size = size + sizeof(size_t);

    // attempt to allocate memory
    void* memory = std::aligned_alloc(static_cast<std::size_t>(alignment), padded_size);
    
    // if memory allocation fails, call handler till nullptr or it works
    std::new_handler handler;
    while((memory == nullptr) && (handler = get_new_handler())){
        handler();
        memory = std::aligned_alloc(static_cast<std::size_t>(alignment), padded_size);
    }

    // if we did allocate memory, save how much and increment usage
    if(memory){
        *(std::size_t*)memory = size;
        memory = (std::size_t*)memory + 1;
        usage += size;
    }

    // return the allocated memory (or nullptr)
    return memory;
}

void deallocate(void* memory){
    // handle nullptr case
    if(memory == nullptr){ return; }

    // get back to 'real' pointer
    memory = (std::size_t*)memory - 1;

    // save the size the memory took up
    std::size_t size = *(std::size_t*)memory;
    usage -= size;

    // free memory
    free(memory);
}

void* operator new(std::size_t size){
    void *memory = allocate(size);

    if(memory == nullptr){
        throw std::bad_alloc();
    }

    return memory;
}

void* operator new(std::size_t size, std::align_val_t alignment){
    void *memory = allocate(size, alignment);

    if(memory == nullptr){
        throw std::bad_alloc();
    }

    return memory;
}

void* operator new(std::size_t size, const std::nothrow_t& tag) noexcept {
    return allocate(size);
}

void* operator new(std::size_t size, std::align_val_t alignment, const std::nothrow_t& tag) noexcept {
    return allocate(size, alignment);
}

void* operator new[](std::size_t size){
    return ::operator new(size);
}

void* operator new[](std::size_t size, std::align_val_t alignment){
    return ::operator new(size, alignment);
}

void* operator new[](std::size_t size, const std::nothrow_t& tag) noexcept {
    return ::operator new(size, tag);
}

void* operator new[](std::size_t size, std::align_val_t alignment, const std::nothrow_t& tag) noexcept {
    return ::operator new(size, alignment, tag);
}

void operator delete(void* memory) noexcept { deallocate(memory); }
void operator delete[](void* memory) noexcept { deallocate(memory); }
void operator delete(void* memory, std::align_val_t) noexcept { deallocate(memory); }
void operator delete[](void* memory, std::align_val_t) noexcept { deallocate(memory); }
void operator delete(void* memory, std::size_t sz) noexcept { deallocate(memory); }
void operator delete[](void* memory, std::size_t sz) noexcept { deallocate(memory); }
void operator delete(void* memory, std::size_t sz, std::align_val_t) noexcept { deallocate(memory); }
void operator delete[](void* memory, std::size_t sz, std::align_val_t) noexcept { deallocate(memory); }
void operator delete(void* memory, const std::nothrow_t&) noexcept { deallocate(memory); }
void operator delete[](void* memory, const std::nothrow_t&) noexcept { deallocate(memory); }
void operator delete(void* memory, std::align_val_t, const std::nothrow_t&) noexcept { deallocate(memory); }
void operator delete[](void* memory, std::align_val_t, const std::nothrow_t&) noexcept { deallocate(memory); }

std::size_t safemem::get_global_new_usage(){
    return usage;
}