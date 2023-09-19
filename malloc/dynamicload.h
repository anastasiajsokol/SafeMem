/**
 *   Get address of symbol at runtime
 *      author      Anastasia Sokol
 *      liscense    Unliscense <https://unlicense.org>
 *      requires    dlfcn.h or windows.h
 *      
 *      WARNING: dynamic_fetch_symbol_address may call malloc
**/

/**
 *  Load the dynamic address of a symbol
 *      param: symbol name to fetch the address of
 *      WARNING: may call malloc
**/
void* dynamic_fetch_symbol_address(const char* symbol);