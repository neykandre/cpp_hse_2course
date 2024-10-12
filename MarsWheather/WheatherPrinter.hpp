#pragma once

#include <memory>

#include "printer.h"
#include "sensor.hpp"

class WeatherPrinterConstructor {
public:
    std::unique_ptr<WeatherPrinter> add_sensor(std::unique_ptr<WeatherPrinter> printer, std::string sensor_name);
    static WeatherPrinterConstructor& get_instance()
    {
        static WeatherPrinterConstructor instance;
        return instance;
    }

    template <Sensor S>
    void register_sensor(std::string sensor_name)
    {
        std::cout << "Registering " << sensor_name << std::endl;
    }

private:
    WeatherPrinterConstructor() {};
    WeatherPrinterConstructor(const WeatherPrinterConstructor&) = delete;
    WeatherPrinterConstructor& operator=(const WeatherPrinterConstructor&) = delete;
    WeatherPrinterConstructor(WeatherPrinterConstructor&&) = delete;
    WeatherPrinterConstructor& operator=(WeatherPrinterConstructor&&) = delete;
};

class WheatherPrinterDecorator : public WeatherPrinter {
public:
    WheatherPrinterDecorator(std::unique_ptr<WeatherPrinter> printer) : printer_(std::move(printer)) {}
    void print_to(std::ostream& stream) override;
private:
    std::unique_ptr<WeatherPrinter> printer_;
};