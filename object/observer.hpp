/**
 *  A class for creating objects that are away of their collective state
 *      author      Anastasia Sokol
 *      liscense    Unliscense <https://unlicense.org>
 *      requires    c++17
 *  
 *  Using an object designed this way it is possible to ensure that every object created is destructed between a given region
 *  This reveals most memory errors made using new/delete and many made using std::allocator or malloc/free
**/

#include <cstdint>

namespace safemem {

/**
 *  Debugging class aware of its own collective state 
**/
class Observer {
    private:
        inline static std::size_t observers = 0;

    public:
        Observer(){ ++observers; }
        Observer(const Observer&){ ++observers; }
        Observer(const Observer&&){ ++observers; }

        Observer& operator =(const Observer&){ return *this; };
        Observer& operator =(const Observer&&){ return *this; };

        ~Observer(){ --observers; }

        static std::size_t global_number_of_observers(){
            return observers;
        }
};

}; // end namespace safemem