#pragma once

#include <cstddef>
#include <expected>

#include "types.hpp"

namespace stdx::details {

// Шаблонный класс для хранения форматирующей строчки и ее особенностей
// ваш код здесь
template <fixed_string str>
class format_string {
    static fixed_string _str{str};

    // Функция для получения количества плейсхолдеров и проверки корректности формирующей строки
    static consteval std::expected<size_t, parse_error> get_number_placeholders() {
        constexpr size_t N = str.size();
        if (!N)
            return 0;
        size_t placeholder_count = 0;
        size_t pos = 0;
        const size_t size = N - 1;  // -1 для игнорирования нуль-терминатора

        while (pos < size) {
            // Пропускаем все символы до '{'
            if (str.data[pos] != '{') {
                ++pos;
                continue;
            }

            // Проверяем незакрытый плейсхолдер
            if (pos + 1 >= size) {
                return std::unexpected(parse_error{"Unclosed last placeholder"});
            }

            // Начало плейсхолдера
            ++placeholder_count;
            ++pos;

            // Проверка спецификатора формата
            if (str.data[pos] == '%') {
                ++pos;
                if (pos >= size) {
                    return std::unexpected(parse_error{"Unclosed last placeholder"});
                }

                // Проверяем допустимые спецификаторы
                const char spec = str.data[pos];
                constexpr char valid_specs[] = {'d', 'u', 'f', 's'};
                bool valid = false;

                for (const char s : valid_specs) {
                    if (spec == s) {
                        valid = true;
                        break;
                    }
                }

                if (!valid) {
                    return std::unexpected(parse_error{"Invalid specifier."});
                }
                ++pos;
            }

            // Проверяем закрывающую скобку
            if (pos >= size || str.data[pos] != '}') {
                return std::unexpected(parse_error{"\'}\' hasn't been found in appropriate place"});
            }
            ++pos;
        }

        return placeholder_count;
    }

    static constexpr std::expected<size_t, parse_error> number_placeholders = get_number_placeholders();
    static_assert(number_placeholders.has_value(), "Invalid format string");

    using placeholder_positions_t = std::array<std::pair<std::size_t, std::size_t>, number_placeholders.value()>;
    // Функция для получения позиций плейсхолдеров
    static placeholder_positions_t get_placeholder_positions() {
        constexpr size_t N = str.size();
        if (!N)
            return 0;

        size_t placeholder_start = 0;
        size_t placeholder_finish = 0;
        placeholder_positions_t result;
        size_t result_index = 0;
        size_t pos = 0;
        const size_t size = N - 1;  // -1 для игнорирования нуль-терминатора

        while (pos < size) {
            // Пропускаем все символы до '{'
            if (str.data[pos] != '{') {
                ++pos;
                continue;
            }
            // Начало плейсхолдера
            placeholder_start = pos;

            while (pos < size && str.data[pos] != '}') {
                ++pos;
            }

            placeholder_finish = pos;
            ++pos;

            result[result_index++] = {placeholder_start, placeholder_finish};
        }
        return result;
    }
    static placeholder_positions_t placeholder_positions = get_placeholder_positions();
};

// Пользовательский литерал
/*
ваш код здесь
ваш код здесь operator"" _fs()  сигнатуру также поменяйте
{
ваш код здесь
}
*/

}  // namespace stdx::details
