#include <iostream>

#include "microfmt.hpp"

template<typename T> void do_not_optimize(const T& val) {
    asm volatile("" : : "g"(val) : "memory");
}

int main() {
    int x = 0;
    int y = 100;
    for(int i = 0; i < 100'000'000; i++) {
        do_not_optimize(microfmt::format("0x{>8:0h} {8:d}", x, y));
        x++;
        y += 2;
    }
}
