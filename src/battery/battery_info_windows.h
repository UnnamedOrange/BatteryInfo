#pragma once

#include <cstddef>
#include <iostream>
#include <string_view>
#include <vector>

#include <Windows.h>
#include <batclass.h>
#include <devguid.h>

#include <setupapi.h> // setupapi.lib

#include "types.h"

namespace orange::battery {
    inline void enumerate_batteries() noexcept {
        HDEVINFO hdevinfo = SetupDiGetClassDevsW(&GUID_DEVCLASS_BATTERY, //
                                                 nullptr,                // Enumerator
                                                 nullptr,                // hwndParent
                                                 DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
        if (hdevinfo == INVALID_HANDLE_VALUE) {
            return;
        }

        // Query each battery device.
        for (auto idx = DWORD{0}; true; idx++) {
            // Get interface data.
            auto did = SP_DEVICE_INTERFACE_DATA{
                .cbSize = sizeof(SP_DEVICE_INTERFACE_DATA),
            };
            {
                const auto result = SetupDiEnumDeviceInterfaces(hdevinfo,               //
                                                                nullptr,                // DeviceInfoData
                                                                &GUID_DEVCLASS_BATTERY, //
                                                                idx,                    //
                                                                &did);
                if (!result) {
                    break; // Regarded as no more items.
                }
            }

            // Get device path.
            DWORD required_size;
            SetupDiGetDeviceInterfaceDetailW(hdevinfo, &did, nullptr, NULL, &required_size, nullptr);
            auto buf = std::vector<std::byte>(required_size);
            auto& didd = *reinterpret_cast<SP_DEVICE_INTERFACE_DETAIL_DATA_W*>(buf.data());
            didd.cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA_W);
            SetupDiGetDeviceInterfaceDetailW(hdevinfo, &did, &didd, required_size, &required_size, nullptr);

            // Open device.
            [[maybe_unused]] const auto _device_path = std::wstring_view{didd.DevicePath};
            HANDLE hdev = CreateFileW(didd.DevicePath,                      //
                                      GENERIC_READ | GENERIC_WRITE,         //
                                      FILE_SHARE_READ | FILE_SHARE_WRITE,   //
                                      nullptr,                              // dwCreationDisposition
                                      OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, //
                                      nullptr);                             // hTemplateFile
            if (hdev == INVALID_HANDLE_VALUE) {
                continue;
            }

            // Query battery information.
            ULONG battery_tag;
            auto timeout_ms = ULONG{0};
            DWORD bytes_returned;
            if (!DeviceIoControl(hdev,                    //
                                 IOCTL_BATTERY_QUERY_TAG, //
                                 &timeout_ms,             //
                                 sizeof(timeout_ms),      //
                                 &battery_tag,            //
                                 sizeof(battery_tag),     //
                                 &bytes_returned,         //
                                 nullptr)) {              // lpOverlapped
                goto FAIL_DEVICE_IO_CONTROL;
            }
            if (!battery_tag) {
                goto FAIL_DEVICE_IO_CONTROL;
            }

            auto bqi = BATTERY_QUERY_INFORMATION{
                .BatteryTag = battery_tag,
            };
            BATTERY_INFORMATION bi;
            if (!DeviceIoControl(hdev,                            //
                                 IOCTL_BATTERY_QUERY_INFORMATION, //
                                 &bqi,                            //
                                 sizeof(bqi),                     //
                                 &bi,                             //
                                 sizeof(bi),                      //
                                 &bytes_returned,                 //
                                 nullptr)) {                      // lpOverlapped
                goto FAIL_DEVICE_IO_CONTROL;
            }

            auto bws = BATTERY_WAIT_STATUS{
                .BatteryTag = battery_tag,
                .Timeout = 0,
            };
            BATTERY_STATUS bs;
            if (!DeviceIoControl(hdev,                       //
                                 IOCTL_BATTERY_QUERY_STATUS, //
                                 &bws,                       //
                                 sizeof(bws),                //
                                 &bs,                        //
                                 sizeof(bs),                 //
                                 &bytes_returned,            //
                                 nullptr)) {                 // lpOverlapped
                goto FAIL_DEVICE_IO_CONTROL;
            }

        FAIL_DEVICE_IO_CONTROL:
            CloseHandle(hdev);
        }

        SetupDiDestroyDeviceInfoList(hdevinfo);
    }
} // namespace orange::battery
