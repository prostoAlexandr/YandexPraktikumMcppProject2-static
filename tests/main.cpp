#include "scan.hpp"
#include <cstdint>
#include <limits>

int main() {
    // Test string type
    {
        using test_type = std::string_view;
        constexpr auto result = stdx::scan<"test {}"_fs, "test view_string_value", test_type>();
        constexpr auto test = std::make_tuple<test_type>("view_string_value");
        static_assert(result.values() == test && std::is_same_v<decltype(result.tuple), decltype(test)>, "Test failed");
    }
    {
        using test_type = std::string_view;
        constexpr auto result = stdx::scan<"test {%s}"_fs, "test view_string_value", test_type>();
        constexpr auto test = std::make_tuple<test_type>("view_string_value");
        static_assert(result.values() == test && std::is_same_v<decltype(result.tuple), decltype(test)>, "Test failed");
    }

    // Test several types
    {
        constexpr auto result = stdx::scan<"test{} {};{}"_fs, "test10 view_string_value;-6", uint8_t, std::string_view, int64_t>();
        constexpr auto test = std::make_tuple<uint8_t, std::string_view, int64_t>(10, "view_string_value", -6);
        static_assert(result.values() == test && std::is_same_v<decltype(result.tuple), decltype(test)>, "Test failed");
    }
    {
        constexpr auto result = stdx::scan<"test{%d} {%u};{%s}"_fs, "test993 42;view_string_value", int16_t, uint8_t, std::string_view>();
        constexpr auto test = std::make_tuple<int16_t, uint8_t, std::string_view>(993, 42, "view_string_value");
        static_assert(result.values() == test && std::is_same_v<decltype(result.tuple), decltype(test)>, "Test failed");
    }

    // Test allowed decimal values
    {
        using test_type = int8_t;
        constexpr auto result = stdx::scan<"test {}"_fs, "test 127", test_type>();
        constexpr auto test = std::make_tuple<test_type>(std::numeric_limits<test_type>::max());
        static_assert(result.values() == test && std::is_same_v<decltype(result.tuple), decltype(test)>, "Test failed");
    }
    {
        using test_type = int8_t;
        constexpr auto result = stdx::scan<"test {}"_fs, "test -128", test_type>();
        constexpr auto test = std::make_tuple<test_type>(std::numeric_limits<test_type>::min());
        static_assert(result.values() == test && std::is_same_v<decltype(result.tuple), decltype(test)>, "Test failed");
    }
    {
        using test_type = int16_t;
        constexpr auto result = stdx::scan<"test {}"_fs, "test 32767", test_type>();
        constexpr auto test = std::make_tuple<test_type>(std::numeric_limits<test_type>::max());
        static_assert(result.values() == test && std::is_same_v<decltype(result.tuple), decltype(test)>, "Test failed");
    }
    {
        using test_type = int16_t;
        constexpr auto result = stdx::scan<"test {}"_fs, "test -32768", test_type>();
        constexpr auto test = std::make_tuple<test_type>(std::numeric_limits<test_type>::min());
        static_assert(result.values() == test && std::is_same_v<decltype(result.tuple), decltype(test)>, "Test failed");
    }
    {
        using test_type = int32_t;
        constexpr auto result = stdx::scan<"test {}"_fs, "test 2147483647", test_type>();
        constexpr auto test = std::make_tuple<test_type>(std::numeric_limits<test_type>::max());
        static_assert(result.values() == test && std::is_same_v<decltype(result.tuple), decltype(test)>, "Test failed");
    }
    {
        using test_type = int32_t;
        constexpr auto result = stdx::scan<"test {}"_fs, "test -2147483648", test_type>();
        constexpr auto test = std::make_tuple<test_type>(std::numeric_limits<test_type>::min());
        static_assert(result.values() == test && std::is_same_v<decltype(result.tuple), decltype(test)>, "Test failed");
    }
    {
        using test_type = int64_t;
        constexpr auto result = stdx::scan<"test {}"_fs, "test 9223372036854775807", test_type>();
        constexpr auto test = std::make_tuple<test_type>(std::numeric_limits<test_type>::max());
        static_assert(result.values() == test && std::is_same_v<decltype(result.tuple), decltype(test)>, "Test failed");
    }
    {
        using test_type = int64_t;
        constexpr auto result = stdx::scan<"test {}"_fs, "test -9223372036854775808", test_type>();
        constexpr auto test = std::make_tuple<test_type>(std::numeric_limits<test_type>::min());
        static_assert(result.values() == test && std::is_same_v<decltype(result.tuple), decltype(test)>, "Test failed");
    }

    // Test allowed decimal values with specifiers
    {
        using test_type = int8_t;
        constexpr auto result = stdx::scan<"test {%d}"_fs, "test 127", test_type>();
        constexpr auto test = std::make_tuple<test_type>(std::numeric_limits<test_type>::max());
        static_assert(result.values() == test && std::is_same_v<decltype(result.tuple), decltype(test)>, "Test failed");
    }
    {
        using test_type = int8_t;
        constexpr auto result = stdx::scan<"test {%d}"_fs, "test -128", test_type>();
        constexpr auto test = std::make_tuple<test_type>(std::numeric_limits<test_type>::min());
        static_assert(result.values() == test && std::is_same_v<decltype(result.tuple), decltype(test)>, "Test failed");
    }
    {
        using test_type = int16_t;
        constexpr auto result = stdx::scan<"test {%d}"_fs, "test 32767", test_type>();
        constexpr auto test = std::make_tuple<test_type>(std::numeric_limits<test_type>::max());
        static_assert(result.values() == test && std::is_same_v<decltype(result.tuple), decltype(test)>, "Test failed");
    }
    {
        using test_type = int16_t;
        constexpr auto result = stdx::scan<"test {%d}"_fs, "test -32768", test_type>();
        constexpr auto test = std::make_tuple<test_type>(std::numeric_limits<test_type>::min());
        static_assert(result.values() == test && std::is_same_v<decltype(result.tuple), decltype(test)>, "Test failed");
    }
    {
        using test_type = int32_t;
        constexpr auto result = stdx::scan<"test {%d}"_fs, "test 2147483647", test_type>();
        constexpr auto test = std::make_tuple<test_type>(std::numeric_limits<test_type>::max());
        static_assert(result.values() == test && std::is_same_v<decltype(result.tuple), decltype(test)>, "Test failed");
    }
    {
        using test_type = int32_t;
        constexpr auto result = stdx::scan<"test {%d}"_fs, "test -2147483648", test_type>();
        constexpr auto test = std::make_tuple<test_type>(std::numeric_limits<test_type>::min());
        static_assert(result.values() == test && std::is_same_v<decltype(result.tuple), decltype(test)>, "Test failed");
    }
    {
        using test_type = int64_t;
        constexpr auto result = stdx::scan<"test {%d}"_fs, "test 9223372036854775807", test_type>();
        constexpr auto test = std::make_tuple<test_type>(std::numeric_limits<test_type>::max());
        static_assert(result.values() == test && std::is_same_v<decltype(result.tuple), decltype(test)>, "Test failed");
    }
    {
        using test_type = int64_t;
        constexpr auto result = stdx::scan<"test {%d}"_fs, "test -9223372036854775808", test_type>();
        constexpr auto test = std::make_tuple<test_type>(std::numeric_limits<test_type>::min());
        static_assert(result.values() == test && std::is_same_v<decltype(result.tuple), decltype(test)>, "Test failed");
    }

    // Test allowed unsigned values
    {
        using test_type = uint8_t;
        constexpr auto result = stdx::scan<"test {}"_fs, "test 0", test_type>();
        constexpr auto test = std::make_tuple<test_type>(std::numeric_limits<test_type>::min());
        static_assert(result.values() == test && std::is_same_v<decltype(result.tuple), decltype(test)>, "Test failed");
    }
    {
        using test_type = uint8_t;
        constexpr auto result = stdx::scan<"test {}"_fs, "test 255", test_type>();
        constexpr auto test = std::make_tuple<test_type>(std::numeric_limits<test_type>::max());
        static_assert(result.values() == test && std::is_same_v<decltype(result.tuple), decltype(test)>, "Test failed");
    }
    {
        using test_type = uint16_t;
        constexpr auto result = stdx::scan<"test {}"_fs, "test 65535", test_type>();
        constexpr auto test = std::make_tuple<test_type>(std::numeric_limits<test_type>::max());
        static_assert(result.values() == test && std::is_same_v<decltype(result.tuple), decltype(test)>, "Test failed");
    }
    {
        using test_type = uint32_t;
        constexpr auto result = stdx::scan<"test {}"_fs, "test 4294967295", test_type>();
        constexpr auto test = std::make_tuple<test_type>(std::numeric_limits<test_type>::max());
        static_assert(result.values() == test && std::is_same_v<decltype(result.tuple), decltype(test)>, "Test failed");
    }
    {
        using test_type = uint64_t;
        constexpr auto result = stdx::scan<"test {}"_fs, "test 18446744073709551615", test_type>();
        constexpr auto test = std::make_tuple<test_type>(std::numeric_limits<test_type>::max());
        static_assert(result.values() == test && std::is_same_v<decltype(result.tuple), decltype(test)>, "Test failed");
    }

    // Test allowed unsigned values with specifiers
    {
        using test_type = uint8_t;
        constexpr auto result = stdx::scan<"test {%u}"_fs, "test 0", test_type>();
        constexpr auto test = std::make_tuple<test_type>(std::numeric_limits<test_type>::min());
        static_assert(result.values() == test && std::is_same_v<decltype(result.tuple), decltype(test)>, "Test failed");
    }
    {
        using test_type = uint8_t;
        constexpr auto result = stdx::scan<"test {%u}"_fs, "test 255", test_type>();
        constexpr auto test = std::make_tuple<test_type>(std::numeric_limits<test_type>::max());
        static_assert(result.values() == test && std::is_same_v<decltype(result.tuple), decltype(test)>, "Test failed");
    }
    {
        using test_type = uint16_t;
        constexpr auto result = stdx::scan<"test {%u}"_fs, "test 65535", test_type>();
        constexpr auto test = std::make_tuple<test_type>(std::numeric_limits<test_type>::max());
        static_assert(result.values() == test && std::is_same_v<decltype(result.tuple), decltype(test)>, "Test failed");
    }
    {
        using test_type = uint32_t;
        constexpr auto result = stdx::scan<"test {%u}"_fs, "test 4294967295", test_type>();
        constexpr auto test = std::make_tuple<test_type>(std::numeric_limits<test_type>::max());
        static_assert(result.values() == test && std::is_same_v<decltype(result.tuple), decltype(test)>, "Test failed");
    }
    {
        using test_type = uint64_t;
        constexpr auto result = stdx::scan<"test {%u}"_fs, "test 18446744073709551615", test_type>();
        constexpr auto test = std::make_tuple<test_type>(std::numeric_limits<test_type>::max());
        static_assert(result.values() == test && std::is_same_v<decltype(result.tuple), decltype(test)>, "Test failed");
    }
}
