/**
 *  Replace default global new and delete operators with a version that keeps track of memory usage
 *      author      Anastasia Sokol
 *      liscense    Unliscense <https://unlicense.org>
 *      requires    c++17
 * 
 *  TODO conditional compilation for other c++ versions
**/

#include <cstdint>

namespace safemem {
    /**
     *  Get the current number of bytes allocated by new
    **/
    std::size_t get_global_new_usage();
};