#pragma once

#include <iostream>

#include <Windows.h>
#include <batclass.h>
#include <devguid.h>

#include <setupapi.h> // setupapi.lib

#include "types.h"

namespace orange::battery {
    inline void enumerate_batteries() noexcept {
        // Enumerate battery devices.
        HDEVINFO hDevInfo = SetupDiGetClassDevsW(&GUID_DEVCLASS_BATTERY, NULL, NULL, DIGCF_PRESENT);
        if (hDevInfo == INVALID_HANDLE_VALUE) {
            return;
        }

        // Query each battery device.
        SP_DEVINFO_DATA devInfoData;
        devInfoData.cbSize = sizeof(devInfoData);
        for (DWORD i = 0; SetupDiEnumDeviceInfo(hDevInfo, i, &devInfoData); i++) {
            // Get device path.
            wchar_t devicePath[MAX_PATH];
            if (!SetupDiGetDeviceRegistryPropertyW(hDevInfo,                            //
                                                   &devInfoData,                        //
                                                   SPDRP_PHYSICAL_DEVICE_OBJECT_NAME,   //
                                                   nullptr,                             //
                                                   reinterpret_cast<BYTE*>(devicePath), //
                                                   sizeof(devicePath),                  //
                                                   NULL)) {
                continue;
            }

            // Open device.
            HANDLE hDevice = CreateFileW(devicePath,                           //
                                         GENERIC_READ | GENERIC_WRITE,         //
                                         FILE_SHARE_READ | FILE_SHARE_WRITE,   //
                                         nullptr,                              //
                                         OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, //
                                         nullptr);
            if (hDevice == INVALID_HANDLE_VALUE) {
                continue;
            }

            // Query battery information.
            BATTERY_QUERY_INFORMATION batteryQueryInfo;
            batteryQueryInfo.BatteryTag = 0;
            batteryQueryInfo.InformationLevel = BatteryInformation;
            DWORD bytesReturned;
            if (!DeviceIoControl(hDevice, IOCTL_BATTERY_QUERY_INFORMATION, &batteryQueryInfo, sizeof(batteryQueryInfo),
                                 nullptr, 0, &bytesReturned, NULL)) {
                goto FAIL_DEVICE_IO_CONTROL;
            }
            BATTERY_INFORMATION batteryInfo;
            if (!DeviceIoControl(hDevice, IOCTL_BATTERY_QUERY_INFORMATION, &batteryQueryInfo, sizeof(batteryQueryInfo),
                                 &batteryInfo, sizeof(batteryInfo), &bytesReturned, NULL)) {
                goto FAIL_DEVICE_IO_CONTROL;
            }

        FAIL_DEVICE_IO_CONTROL:
            CloseHandle(hDevice);
        }

        SetupDiDestroyDeviceInfoList(hDevInfo);
    }
} // namespace orange::battery
