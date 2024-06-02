#include <gtest/gtest.h>

#define CPP_17 0
#if defined(_MSVC_LANG) && _MSVC_LANG >= 201703L
 #undef CPP_17
 #define CPP_17 1
#elif __cplusplus >= 201703L
 #undef CPP_17
 #define CPP_17 1
#endif

#include <algorithm>
#include <iomanip>
#include <memory>
#include <sstream>
#if CPP_17
 #include <string_view>
#endif
#include <string>
#include <vector>
using namespace std::literals;

#include "microfmt.hpp"

#if CPP_17
// String views from ""sv seem to be null terminated and asan doesn't compilain about one-past the end access
// This is just a utility to create string views that will error with out of bounds access
std::vector<std::unique_ptr<char[]>> string_store;
std::string_view operator""_sv(const char* str, std::size_t length) {
    auto buffer = std::make_unique<char[]>(length);
    std::memcpy(buffer.get(), str, length);
    std::string_view sv{buffer.get(), buffer.get() + length};
    string_store.push_back(std::move(buffer));
    return sv;
}
#endif

TEST(MicrofmtTest, basic) {
    EXPECT_EQ(microfmt::format("test"), "test");
    EXPECT_EQ(microfmt::format("a {} b", "foo"), "a foo b");
    EXPECT_EQ(microfmt::format("a {} b", "foo"s), "a foo b");
    EXPECT_EQ(microfmt::format("a {} b", 'x'), "a x b");
    #if CPP_17
    EXPECT_EQ(microfmt::format("a {} b", "foo"sv), "a foo b");
    EXPECT_EQ(microfmt::format("a {} b"sv, 'x'), "a x b");
    #endif
}

TEST(MicrofmtTest, width_and_alignment) {
    EXPECT_EQ(microfmt::format("a {20} b", "foo"), "a foo                  b");
    EXPECT_EQ(microfmt::format("a {<20} b", "foo"), "a foo                  b");
    EXPECT_EQ(microfmt::format("a {>20} b", "foo"), "a                  foo b");
    // overflowing width
    EXPECT_EQ(microfmt::format("a {<2} b", "foo"), "a foo b");
    EXPECT_EQ(microfmt::format("a {>2} b", "foo"), "a foo b");
    // trailing :'s
    #if CPP_17
    EXPECT_EQ(microfmt::format("a {20:} b"_sv, "foo"), "a foo                  b");
    EXPECT_EQ(microfmt::format("a {>20:} b"_sv, "foo"), "a                  foo b");
    #endif
}

TEST(MicrofmtTest, basic_numbers) {
    EXPECT_EQ(microfmt::format("a {<} {>} b", 1ULL<<62, -(1LL<<60)), "a 4611686018427387904 -1152921504606846976 b");
    EXPECT_EQ(microfmt::format("a {} b", (short)12), "a 12 b");
    EXPECT_EQ(microfmt::format("a {} b", (long)12), "a 12 b");
    EXPECT_EQ(microfmt::format("a {} b", (unsigned char)12), "a 12 b");
    EXPECT_EQ(microfmt::format("a {} b", (unsigned short)12), "a 12 b");
}

TEST(MicrofmtTest, basic_bases) {
    EXPECT_EQ(microfmt::format("a {:d} b", 122), "a 122 b");
    EXPECT_EQ(microfmt::format("a {:h} b", 122), "a 7a b");
    EXPECT_EQ(microfmt::format("a {:H} b", 122), "a 7A b");
    EXPECT_EQ(microfmt::format("a {:o} b", 122), "a 172 b");
    EXPECT_EQ(microfmt::format("a {:b} b", 122), "a 1111010 b");
}

TEST(MicrofmtTest, padding) {
    EXPECT_EQ(microfmt::format("a {:0d} b", "foo"), "a foo b");
    EXPECT_EQ(microfmt::format("a {10:0d} b", "foo"), "a foo0000000 b");
    EXPECT_EQ(microfmt::format("a {>10:0d} b", 122), "a 0000000122 b");
}

TEST(MicrofmtTest, variable_width) {
    EXPECT_EQ(microfmt::format("a {>{}} b", 14, 122), "a            122 b");
    EXPECT_EQ(microfmt::format("a {>{}} b", (unsigned)14, 122), "a            122 b");
}

TEST(MicrofmtTest, base_conversion) {
    EXPECT_EQ(microfmt::format("{:h}", 0), "0");
    EXPECT_EQ(microfmt::format("{:h}", 1), "1");
    EXPECT_EQ(microfmt::format("{:h}", 15), "f");
    EXPECT_EQ(microfmt::format("{:h}", 16), "10");
    EXPECT_EQ(microfmt::format("{:h}", 17), "11");
    EXPECT_EQ(microfmt::format("{:h}", 0xffffffffffffffff), "ffffffffffffffff");
}

std::string to_string(std::uint64_t value, int base) {
    std::ostringstream oss;
    oss << std::setbase(base) << value;
    return std::move(oss).str();
}

TEST(MicrofmtTest, base_16) {
    for(unsigned num = 0; num < 10'000; num++) {
        EXPECT_EQ(microfmt::format("{:h}", num), to_string(num, 16));
    }
    for(unsigned num = 0xfffff000; num; num++) {
        EXPECT_EQ(microfmt::format("{:h}", num), to_string(num, 16));
    }
}

TEST(MicrofmtTest, base_8) {
    for(unsigned num = 0; num < 10'000; num++) {
        EXPECT_EQ(microfmt::format("{:o}", num), to_string(num, 8));
    }
    for(unsigned num = 0xfffff000; num; num++) {
        EXPECT_EQ(microfmt::format("{:o}", num), to_string(num, 8));
    }
}

std::string to_binary(std::uint64_t value) {
    if(value == 0) {
        return "0";
    } else {
        std::string str;
        while(value) {
            str += (value & 1) + '0';
            value >>= 1;
        }
        std::reverse(str.begin(), str.end());
        return str;
    }
}

TEST(MicrofmtTest, base_2) {
    for(unsigned num = 0; num < 10'000; num++) {
        EXPECT_EQ(microfmt::format("{:b}", num), to_binary(num));
    }
    for(unsigned num = 0xfffff000; num; num++) {
        EXPECT_EQ(microfmt::format("{:b}", num), to_binary(num));
    }
}

TEST(MicrofmtTest, escapes) {
    EXPECT_EQ(microfmt::format("test {{ aagasdg }}"), "test { aagasdg }");
    EXPECT_EQ(microfmt::format("test {{ aagasdg }}", 1), "test { aagasdg }");
    #if CPP_17
    EXPECT_EQ(microfmt::format("test {{ aagasdg }}"sv), "test { aagasdg }");
    EXPECT_EQ(microfmt::format("test {{ aagasdg }}"sv, 1), "test { aagasdg }");
    #endif
}

TEST(MicrofmtTest, adjacent_format) {
    EXPECT_EQ(microfmt::format("{}{}", "foo", "bar"), "foobar");
}

#if CPP_17
TEST(MicrofmtTest, erroneous_input) {
    // base on non-numeric
    EXPECT_EQ(microfmt::format("{>10:h}"_sv, "foo"), "       foo");
    // unknown base
    EXPECT_EQ(microfmt::format("{>10:q}"_sv, 20), "qqqqqqqq20");
    EXPECT_EQ(microfmt::format("{>10:qq}"_sv, 20), "qqqqqqqq20");
    // non-numeric variable width
    EXPECT_EQ(microfmt::format("a {>{}} b"_sv, "foo", 122), "a 122 b");
    // missing variable width and missing input
    EXPECT_EQ(microfmt::format("a {>{}} b"_sv), "a  b");
    // extra inputs
    EXPECT_EQ(microfmt::format("a {} b"_sv, "foo", "bar", "baz"), "a foo b");
    // argument eater
    EXPECT_EQ(microfmt::format("a {<{}{<{}{} {} b"_sv, "foo", "bar", "baz"), "a {<bar{<  b");
    // malformed format strings
    EXPECT_EQ(microfmt::format("a { b", "foo", 122), "a { b");
    EXPECT_EQ(microfmt::format("a {<"_sv), "a {<");
    EXPECT_EQ(microfmt::format("a {<a"_sv), "a {<a");
    EXPECT_EQ(microfmt::format("a {<a}"_sv), "a {<a}");
    EXPECT_EQ(microfmt::format("a {<{"_sv), "a {<{");
    EXPECT_EQ(microfmt::format("a {<{}"_sv, 12, 20), "a {<20");
    EXPECT_EQ(microfmt::format("a {<{}:"_sv, 12, 20), "a {<20:");
    EXPECT_EQ(microfmt::format("{>10:q"_sv, 20), "{>10:q");
    EXPECT_EQ(microfmt::format("{>10:qq"_sv, 20), "{>10:qq");
    EXPECT_EQ(microfmt::format("{>10:qaaaaaa"_sv, 20), "{>10:qaaaaaa");
    EXPECT_EQ(microfmt::format("{>10:qqaaaaaa"_sv, 20), "{>10:qqaaaaaa");
}
#endif
