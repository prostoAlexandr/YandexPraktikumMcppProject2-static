#pragma once

#include <charconv>
#include <concepts>
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <system_error>
#include <type_traits>

#include "StrToNum.h"
#include "format_string.hpp"
#include "types.hpp"

namespace stdx::details {

// Шаблонная функция, возвращающая пару позиций в строке с исходными данными, соотвествующих I-ому плейсхолдеру
template <int I, format_string fmt, fixed_string source>
consteval auto get_current_source_for_parsing() {
    static_assert(I >= 0 && I < fmt.number_placeholders, "Invalid placeholder index");

    constexpr auto to_sv = [](const auto &fs) { return std::string_view(fs.data, fs.size() - 1); };

    constexpr auto fmt_sv = to_sv(fmt.fmt);
    constexpr auto src_sv = to_sv(source);
    constexpr auto &positions = fmt.placeholder_positions;

    // Получаем границы текущего плейсхолдера в формате
    constexpr auto pos_i = positions[I];
    constexpr size_t fmt_start = pos_i.first, fmt_end = pos_i.second;

    // Находим начало в исходной строке
    constexpr auto src_start = [&] {
        if constexpr (I == 0) {
            return fmt_start;
        } else {
            // Находим конец предыдущего плейсхолдера в исходной строке
            constexpr auto prev_bounds = get_current_source_for_parsing<I - 1, fmt, source>();
            const auto prev_end = prev_bounds.second;

            // Получаем разделитель между текущим и предыдущим плейсхолдерами
            constexpr auto prev_fmt_end = positions[I - 1].second;
            constexpr auto sep = fmt_sv.substr(prev_fmt_end + 1, fmt_start - (prev_fmt_end + 1));

            // Ищем разделитель после предыдущего значения
            auto pos = src_sv.find(sep, prev_end);
            return pos != std::string_view::npos ? pos + sep.size() : src_sv.size();
        }
    }();

    // Находим конец в исходной строке
    constexpr auto src_end = [&] {
        // Получаем разделитель после текущего плейсхолдера
        if constexpr (fmt_end == (fmt_sv.size() - 1)) {
            return src_sv.size();
        }
        constexpr auto sep =
            fmt_sv.substr(fmt_end + 1, (I < fmt.number_placeholders - 1) ? positions[I + 1].first - (fmt_end + 1)
                                                                         : fmt_sv.size() - (fmt_end + 1));
        // Ищем разделитель после текущего значения
        constexpr auto pos = src_sv.find(sep, src_start);
        return pos != std::string_view::npos ? pos : src_sv.size();
    }();
    return std::pair{src_start, src_end};
}

// Реализуйте семейство функций parse_value
template <fixed_string source, std::integral TargetT>
consteval TargetT parse_value() {
    TargetT parsed = 0;
    auto result = std::from_chars(source.data, source.data + source.size(), parsed);
    if (!(bool)result) {
        throw parse_error{"Error parsing value"};
    }
    return parsed;
};

template <typename T>
concept StringvType = std::is_same_v<T, std::string_view>;

template <fixed_string source, StringvType TargetT>
consteval TargetT parse_value() {
    return TargetT(source.data, source.data + source.size());
}

enum SupportedTypes : char { NOT_SET = '?', SIGNED = 'd', UNSIGNED = 'u', STRINGV = 's' };

// Шаблонная функция, выполняющая преобразования исходных данных в конкретный тип на основе I-го плейсхолдера
template <int I, format_string fmt, fixed_string source, typename TargetT>
consteval TargetT parse_input() {
    constexpr auto curr_source = get_current_source_for_parsing<I, fmt, source>();
    constexpr fixed_string<std::remove_extent_t<decltype(source.data)>, curr_source.second - curr_source.first> fs(
        source.data + curr_source.first, source.data + curr_source.second);
    constexpr auto format_specifier = [&] {
        constexpr auto pos_i = fmt.placeholder_positions[I];
        constexpr size_t fmt_start = pos_i.first, fmt_end = pos_i.second;
        return fmt_end - 1 != fmt_start ? fmt.fmt.data[fmt_end - 1] : SupportedTypes::NOT_SET;
    }();

    using no_cvr_target = std::remove_cvref_t<TargetT>;
    if constexpr (!((format_specifier == SupportedTypes::SIGNED &&
                     (std::is_same_v<no_cvr_target, int8_t> || std::is_same_v<no_cvr_target, int16_t> ||
                      std::is_same_v<no_cvr_target, int32_t> || std::is_same_v<no_cvr_target, int64_t>)) ||

                    (format_specifier == SupportedTypes::UNSIGNED &&
                     (std::is_same_v<no_cvr_target, uint8_t> || std::is_same_v<no_cvr_target, uint16_t> ||
                      std::is_same_v<no_cvr_target, uint32_t> || std::is_same_v<no_cvr_target, uint64_t>)) ||

                    (format_specifier == SupportedTypes::STRINGV && std::is_same_v<no_cvr_target, std::string_view>) ||

                    format_specifier == SupportedTypes::NOT_SET)) {
        static_assert(false, "Format specifier - variable type mismatch");
    }
    return parse_value<fs, TargetT>();
}

}  // namespace stdx::details
