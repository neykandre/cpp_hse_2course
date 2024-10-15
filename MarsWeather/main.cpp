#include "printer.h"
#include "solution.h"

#include <cassert>
#include <chrono>
#include <iostream>
#include <memory>
#include <sstream>
#include <thread>
#include <utility>
#include <vector>

int main() {
    struct LongNameSensor {
        int measure() {
            return 42;
        }
    };

    std::string long_name(50, 'a'); // Создаем имя длиной 51 символ

    try {
        SensorRegistrator<LongNameSensor> reg_long(long_name);
        auto printer = std::make_unique<WeatherPrinter>();
        printer->print_to(std::cout);
        printer      = WeatherPrinterConstructor::get_instance().add_sensor(
            std::move(printer), long_name); // Имя больше 50 байт
        printer->print_to(std::cout);
    } catch (const std::invalid_argument& e) {
        std::cout << "Caught invalid_argument for long name sensor" << std::endl;
    }
}