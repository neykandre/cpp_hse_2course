#include "sensor.hpp"
#include "WheatherPrinter.hpp"

class IncrementSensor {
public:
    int measure() {
        return cnt++;
    }
private:
    int cnt = 0;
};

int main() {
    WeatherPrinterConstructor::get_instance().register_sensor<IncrementSensor>("increment");

}