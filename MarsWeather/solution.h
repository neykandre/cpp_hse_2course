#pragma once

#include <memory>
#include <type_traits>
#include <unordered_map>

#include "printer.h"
#include "stdexcept"

template <class T>
concept Sensor =
    std::is_default_constructible_v<T> && requires(std::ostream& out, T& sensor) {
        { out << sensor.measure() };
    };

class WeatherPrinterConstructor {
  public:
    WeatherPrinterConstructor(const WeatherPrinterConstructor&) = delete;
    // WeatherPrinterConstructor(WeatherPrinterConstructor&&)      = delete;

    std::unique_ptr<WeatherPrinter> add_sensor(
        std::unique_ptr<WeatherPrinter> printer, const std::string& sensor_name) {
        if (сreators_map.find(sensor_name) == сreators_map.end()) {
            throw std::invalid_argument(sensor_name + " is not registered");
        }
        if (!printer) {
            throw std::invalid_argument("Printer is null");
        }

        return сreators_map[sensor_name](sensor_name, std::move(printer));
    }

    static WeatherPrinterConstructor& get_instance() {
        static WeatherPrinterConstructor instance;
        return instance;
    }

    template <Sensor S>
    void register_sensor(const std::string& sensor_name) {
        auto& current_creator = сreators_map[std::move(sensor_name)];
        auto creator          = &create_decorator<S>;

        if (current_creator && current_creator != creator) {
            throw std::invalid_argument("Another " + sensor_name +
                                        " is already registered");
        }

        if (current_creator != creator) {
            current_creator = creator;
        }
    }

  private:
    WeatherPrinterConstructor() = default;

    std::unordered_map<std::string,
                       std::unique_ptr<WeatherPrinter> (*)(
                           const std::string&, std::unique_ptr<WeatherPrinter>)>
        сreators_map;

    template <Sensor S>
    static std::unique_ptr<WeatherPrinter> create_decorator(
        const std::string& sensor_name, std::unique_ptr<WeatherPrinter> printer) {
        return std::make_unique<WeatherPrinterDecorator<S>>(std::move(sensor_name),
                                                            std::move(printer));
    }

    template <Sensor S>
    class WeatherPrinterDecorator : public WeatherPrinter {
      public:
        WeatherPrinterDecorator(std::string sensor_name,
                                std::unique_ptr<WeatherPrinter> printer)
            : mSensor_name(sensor_name),
              mPrinter(std::move(printer)),
              mSensor(std::make_unique<S>()) {};

        void print_to(std::ostream& stream) override {
            mPrinter->print_to(stream);
            stream << mSensor_name << ": " << mSensor->measure() << '\n';
        }

      private:
        std::string mSensor_name;
        std::unique_ptr<S> mSensor;
        std::unique_ptr<WeatherPrinter> mPrinter;
    };
};

template <Sensor S>
class SensorRegistrator {
  public:
    SensorRegistrator(std::string sensor_name) {
        WeatherPrinterConstructor::get_instance().register_sensor<S>(
            std::move(sensor_name));
    }
};