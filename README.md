# SafeMem

c++ standalone runtime memory checker

## Tests

#### new / delete

To check for errors relating to the usage of new / delete (and anything that uses them, such as the default std::allocator) you can use the core module.

There are two main components, the header declares a single function

    std::size_t safemem::get_global_new_usage();

This returns the number of bytes the program has currently allocated using new/delete at a given point in time.

Recommended usage is to read this before and after a block of code to make sure no memory was leaked inside that block.

For example

```
std::size_t start = safemem::get_global_new_usage();
{
    // do something
}
std::size_t end = safemem::get_global_new_usage();

bool ok = !(end - start);
```

For a full example see tests/get_global_new_usage_test.cpp

Compile using `g++ core/safemem.cpp tests/get_global_new_usage_test.cpp -o global_usage_test`

### object initialization and destruction

For errors relating to improper internal managment of objects you can use the object module.

This defines one header with a simple 'Observer' object that is self aware of how many of itself there are at a given time.

To check that all objects that are initialized are also destructed within a given block, you can use this pattern.

```
std::size_t start = safemem::Observer::global_number_of_observers();
{
    // do something
}
std::size_t end = safemem::Observer::global_number_of_observers();

bool ok = !(end - start);
```

This will catch most memory errors (especially if new/delete are used) as well as errors which might not be strictly related to memory.

A full example can be found at tests/observers.cpp

Compile using `g++ tests/observers.cpp -o observers_test`

### memory allocating using malloc/free

Unfortunately, unlike new / delete overwriting malloc / free is much harder and impossible to do in a standard way.

On Linux or Unix systems this can be done using LD_PRELOAD

On MacOS this can be done using DYLD_INSERT_LIBRARIES

On Windows you can install Linux then use LD_PRELOAD

This library's implementation of memory checked malloc / free is still in progress.

If you know of any solutions for Windows please let me know. (_CrtSetAllocHook looks promising)

### memory allocating using inline assembly and os calls

Use valgrind or a different tool.