#include <cstdlib>
#include <cstdio>

int main(){
    void* mem = malloc(8);
    std::printf("mem: %p\n", mem);
    return 0;
}