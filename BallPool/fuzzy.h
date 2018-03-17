#pragma once

#include <cmath>

// Сравнение с допустимой абсолютной погрешностью
inline bool areCloseAbsolute(float a, float b, float tolerance = 0.001f)
{
    return std::abs(a - b) < tolerance;
}

// Сравнение с допустимой относительной погрешностью
inline bool areCloseRelative(float a, float b, float tolerance = 0.001f)
{
    return std::abs((a - b) / b) < tolerance;
}

// Сравнение двух чисел с плавающей точкой с допустимой погрешностью 0.001
inline bool areFuzzyEqual(float a, float b)
{
    constexpr float tolerance = 0.001f;
    if (std::abs(b) > 1.f)
    {
        return areCloseRelative(a, b, tolerance);
    }
    return areCloseAbsolute(a, b, tolerance);
}
