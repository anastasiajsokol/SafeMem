/**
 *  Show the usage of safemem::get_global_new_usage() for detecting memory leaks
**/

#include <iostream>
#include <cstdlib>

#include "../core/safemem.h"

int main(){
    std::size_t starting_usage = safemem::get_global_new_usage();

    std::uint8_t *memory = new std::uint8_t[64];

    std::size_t mid_usage = safemem::get_global_new_usage();

    delete[] memory;

    std::size_t end_usage = safemem::get_global_new_usage();

    std::cout << "Starting usage: " << starting_usage << "\nMid usage: " << mid_usage << "\nEnd usage: " << end_usage << "\nYou " << ((end_usage - starting_usage) ? "have a memory leak" : "are all good") << std::endl;

    return EXIT_SUCCESS;
}