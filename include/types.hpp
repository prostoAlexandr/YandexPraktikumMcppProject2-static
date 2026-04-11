#pragma once

#include <algorithm>
#include <cstddef>
#include <tuple>
#include <utility>

namespace stdx::details {

// Шаблонный класс, хранящий C-style строку фиксированной длины

// ваш код здесь
template <typename CharT, std::size_t N>
struct fixed_string {
    CharT _data[N];

    constexpr fixed_string(const CharT (&str)[N]) { std::copy_n(str, N, _data); }

    template <std::size_t M>
    constexpr fixed_string(const CharT (&str)[M]) {
        static_assert(M <= N, "Passed array is too long");
        std::copy_n(str, N, _data);
    }

    constexpr fixed_string(CharT *begin, CharT *end) {
        for (std::size_t i = 0; i < N && begin != end; ++i, std::next(begin)) {
            _data[i] = *begin;
        }
    }

    constexpr const CharT *data() const { return _data; }

    constexpr std::size_t size() const { return N; }
};

// Шаблонный класс, хранящий fixed_string достаточной длины для хранения ошибки парсинга

// ваш код здесь
struct parse_error : public fixed_string<char, 128> {};

// Шаблонный класс для хранения результатов парсинга

template <typename... Ts>
struct scan_result {
    // ваш код здесь
    // измените реализацию
    int i;
    std::tuple<Ts...> tuple;
    std::tuple<Ts...> &values() { return tuple; }
};

}  // namespace stdx::details
