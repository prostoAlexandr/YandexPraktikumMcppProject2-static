#pragma once

#include <algorithm>
#include <cstddef>
#include <tuple>
#include <utility>

namespace stdx::details {

// Шаблонный класс, хранящий C-style строку фиксированной длины
template <typename CharT, std::size_t N>
struct fixed_string {
    CharT data[N];

    constexpr fixed_string(const CharT *str) { std::copy_n(str, N, data); }

    template <std::size_t M>
    constexpr fixed_string(const CharT (&str)[M]) {
        static_assert(M <= N, "Passed array is too long");
        std::copy_n(str, M, data);
    }

    constexpr fixed_string(const CharT *begin, const CharT *end) {
        for (std::size_t i = 0; i < N && begin != end; ++i, ++begin) {
            data[i] = *begin;
        }
    }

    constexpr std::size_t size() const { return N; }
};

// deduction guide для string literal
template <typename CharT, std::size_t N>
fixed_string(const CharT (&)[N]) -> fixed_string<char, N>;

// Шаблонный класс, хранящий fixed_string достаточной длины для хранения ошибки парсинга
struct parse_error : public fixed_string<char, 128> {};

// Шаблонный класс для хранения результатов парсинга
template <typename... Ts>
struct scan_result {
    constexpr scan_result(std::tuple<Ts...> &&t) : tuple(t) {}
    const std::tuple<Ts...> tuple;
    constexpr auto &values() const { return tuple; }
};

}  // namespace stdx::details
