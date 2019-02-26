#pragma once

bool is_high_surrogate(char16_t value) noexcept;
bool is_hangul(char16_t value) noexcept;
char32_t get_chosung(char16_t value) noexcept;