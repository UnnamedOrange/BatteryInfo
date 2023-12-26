#include <cstdlib>
#include <print>
#include <thread>

#include <battery_info/battery_info_manager.h>

using namespace std::literals;

using orange::battery::BatteryInfo;
using orange::battery::BatteryInfoManager;

int main() {
    while (true) {
        BatteryInfoManager::query_battery_info(0).transform([](const BatteryInfo& it) {
            std::system("cls");
            std::println("Full charged capacity: {}", it.full_charged_capacity);
            std::println("Designed capacity: {}", it.designed_capacity);
            std::println("Current capacity: {}", it.capacity);
            std::println("Rate: {}", it.rate);
        });
        std::this_thread::sleep_for(1s);
    }
}
