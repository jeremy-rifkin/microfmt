#include <iostream>

#include "microfmt.hpp"

int main() {
    microfmt::print("foo {>20} bar {} baz\n", 1, std::string("hi"));
    microfmt::print("foo {20} bar {10:0} baz\n", 1, std::string("hi"));
    microfmt::print("foo {20} bar {>10:0} baz\n", 1, std::string("hi"));
    microfmt::print("{: b} {{ {:o} }} {: h}\n", 42, 42, 42);
}
