#pragma once

#include <type_traits>

#include "printer.h"

template <class T>
concept Sensor = std::is_default_constructible_v<T> && requires(std::ostream& out, T& sensor) {
    { out << sensor.measure() };
};
