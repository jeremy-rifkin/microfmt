#include <iostream>

#include "microfmt.hpp"

int main() {
    std::cout<<microfmt::format("foo {>20} bar {} baz\n", 1, std::string("hi"));
    std::cout<<microfmt::format("foo {20} bar {10:0} baz\n", 1, std::string("hi"));
    std::cout<<microfmt::format("foo {20} bar {>10:0} baz\n", 1, std::string("hi"));
    std::cout<<microfmt::format("{: b} {{ {:o} }} {: h}\n", 42, 42, 42);
}
