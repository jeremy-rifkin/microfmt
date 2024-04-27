#include <gtest/gtest.h>

#include <algorithm>
#include <iomanip>
#include <sstream>
#include <string_view>
#include <string>
using namespace std::literals;

#include "microfmt.hpp"

TEST(MicrofmtTest, Basic) {
  EXPECT_EQ(microfmt::format("test"), "test");
  EXPECT_EQ(microfmt::format("a {} b", "foo"), "a foo b");
  EXPECT_EQ(microfmt::format("a {} b", "foo"s), "a foo b");
  EXPECT_EQ(microfmt::format("a {} b", "foo"sv), "a foo b");
  EXPECT_EQ(microfmt::format("a {} b", 'x'), "a x b");
}

TEST(MicrofmtTest, alignment) {
  EXPECT_EQ(microfmt::format("a {20} b", "foo"), "a foo                  b");
  EXPECT_EQ(microfmt::format("a {<20} b", "foo"), "a foo                  b");
  EXPECT_EQ(microfmt::format("a {>20} b", "foo"), "a                  foo b");
}

TEST(MicrofmtTest, basic_numbers) {
  EXPECT_EQ(microfmt::format("a {<} {>} b", 1ULL<<62, -(1LL<<60)), "a 4611686018427387904 -1152921504606846976 b");
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
}
