#pragma once

#include <cstddef>
#include <tuple>

#include "format_string.hpp"
#include "parse.hpp"
#include "types.hpp"
#include <utility>

namespace stdx {

namespace details {

template <format_string fmt, fixed_string source, typename... Ts, std::size_t... I>
consteval auto scan_impl(std::index_sequence<I...>) {
    constexpr scan_result<Ts...> result(std::make_tuple<Ts...>(parse_input<I, fmt, source, Ts>()...));
    return result;
}

}  // namespace details

// Главная функция
template <details::format_string fmt, details::fixed_string source, typename... Ts>
consteval auto scan() {  // передайте пакет параметров в scan_result
                         // измените реализацию
    return details::scan_impl<fmt, source, Ts...>(std::index_sequence_for<Ts...>{});
}

}  // namespace stdx