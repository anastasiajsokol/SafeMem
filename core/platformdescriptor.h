/**
 *   Get the standard platform implementation of c memory functions
 *      author      Anastasia Sokol
 *      liscense    Unliscense <https://unlicense.org>
**/

#include "dynamicload.h"
#include <cstdint>

namespace safemem {

/**
 *  Store a copy to the standard memory functions
 *      caution: zero-size singleton, constructor must be called before pointers accessed
**/
struct PlatformDescriptor {
    using aligned_alloc_t   = void*(std::size_t, std::size_t);
    using calloc_t          = void*(std::size_t, std::size_t);
    using realloc_t         = void*(void*, std::size_t);
    using malloc_t          = void*(std::size_t);
    using free_t            = void*(void*);

    inline static aligned_alloc_t *aligned_alloc    = nullptr;
    inline static realloc_t *realloc                = nullptr;
    inline static calloc_t *calloc                  = nullptr;
    inline static malloc_t *malloc                  = nullptr;
    inline static free_t *free                      = nullptr;

    PlatformDescriptor() = delete;

    /**
     *  Load dynamic addresses into object
     *      returns true if PlatformDescriptor references are valid
     *      returns false every other case
     *  note that it is safe to call multiple times, subsequent loads will be ignored
    **/
    static bool load(){
        if(status == PlatformStatus::unset){
            status          = PlatformStatus::loading;

            aligned_alloc   = (aligned_alloc_t*)dynamic_fetch_symbol_address("aligned_alloc");
            realloc         = (realloc_t*)dynamic_fetch_symbol_address("realloc");
            calloc          = (calloc_t*)dynamic_fetch_symbol_address("calloc");
            malloc          = (malloc_t*)dynamic_fetch_symbol_address("malloc");
            free            = (free_t*)dynamic_fetch_symbol_address("free");

            const bool ok   = aligned_alloc && realloc && calloc && malloc && free;
            status          = ok ? PlatformStatus::set : PlatformStatus::panic;
        }

        return status == PlatformStatus::set;
    }

    private:
        enum class PlatformStatus {
            unset,
            loading,
            set,
            panic
        };

        inline static PlatformStatus status = PlatformStatus::unset;
};

}; // end namespace safemem