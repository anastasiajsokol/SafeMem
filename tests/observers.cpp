/**
 *  Demonstrate use of observer class for detecting object missuse 
**/

#include <iostream>
#include <cstdlib>

#include "../object/observer.hpp"

int main(){
    using safemem::Observer;
    std::size_t start, end;

    // example of proper usage new/delete
    start = Observer::global_number_of_observers();
    {
        Observer* observers = new Observer[10];
        delete[] observers;
    }
    end = Observer::global_number_of_observers();

    std::cout << "Example 1: " << ((end - start) ? "failed" : "ok") << '\n';

    // example of bad usage of new/delete
    start = Observer::global_number_of_observers();
    {
        Observer* observers = new Observer[10];
        observers = new Observer[8];
        delete[] observers;
    }
    end = Observer::global_number_of_observers();

    std::cout << "Example 2: " << ((end - start) ? "failed" : "ok") << '\n';

    // example of good usage of std::allocator
    start = Observer::global_number_of_observers();
    {
        std::allocator<Observer> alloc;
        Observer* observers = alloc.allocate(10);
        for(std::size_t i = 0; i < 10; ++i){
            new (observers + i) Observer();
        }
        for(std::size_t i = 0; i < 10; ++i){
            observers[i].~Observer();
        }
        alloc.deallocate(observers, 10);
    }
    end = Observer::global_number_of_observers();

    std::cout << "Example 3: " << ((end - start) ? "failed" : "ok") << '\n';

    // example of bad usage of std::allocator
    start = Observer::global_number_of_observers();
    {
        std::allocator<Observer> alloc;
        Observer* observers = alloc.allocate(10);
        for(std::size_t i = 0; i < 10; ++i){
            new (observers + i) Observer();
        }
        alloc.deallocate(observers, 10);
    }
    end = Observer::global_number_of_observers();

    std::cout << "Example 4: " << ((end - start) ? "failed" : "ok") << '\n';

    return EXIT_SUCCESS;
}