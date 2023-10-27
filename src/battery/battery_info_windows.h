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
    template <typename T, intptr_t IH>
    class AbstractUniqueHandle {
        using Self = AbstractUniqueHandle<T, IH>;

    public:
        using Handle = T;
        static constexpr auto INVALID_HANDLE = IH;

    protected:
        Handle handle;

    public:
        AbstractUniqueHandle() noexcept : handle(Handle(INVALID_HANDLE)) {}
        AbstractUniqueHandle(const Self&) = delete;
        AbstractUniqueHandle(Self&& other) noexcept : AbstractUniqueHandle() {
            *this = std::move(other);
        }
        Self& operator=(Self&& other) noexcept {
            using std::swap;
            swap(this->handle, other.handle);
            return *this;
        }

        virtual ~AbstractUniqueHandle() = default;

        AbstractUniqueHandle(Handle handle) : handle(handle) {}

    public:
        bool empty() const noexcept {
            return handle == Handle(INVALID_HANDLE);
        }
        T get() const noexcept {
            return handle;
        }
        explicit operator bool() const noexcept {
            return !empty();
        }
        operator T() const noexcept {
            return handle;
        }
    };

    class UniqueHandleDevInfo final : public AbstractUniqueHandle<HDEVINFO, -1> {
        using Self = UniqueHandleDevInfo;
        using Super = AbstractUniqueHandle<HDEVINFO, -1>;

    public:
        using Super::AbstractUniqueHandle;
        ~UniqueHandleDevInfo() {
            reset();
        }

    public:
        void reset() noexcept {
            if (!empty()) {
                SetupDiDestroyDeviceInfoList(Super::handle);
                handle = Handle(INVALID_HANDLE);
            }
        }
    };

    class UniqueHandleX final : public AbstractUniqueHandle<HANDLE, -1> {
        using Self = UniqueHandleDevInfo;
        using Super = AbstractUniqueHandle<HANDLE, -1>;

    public:
        using Super::AbstractUniqueHandle;
        ~UniqueHandleX() {
            reset();
        }

    public:
        void reset() noexcept {
            if (!empty()) {
                CloseHandle(Super::handle);
                handle = Handle(INVALID_HANDLE);
            }
        }
    };

    inline void enumerate_batteries() noexcept {
        auto hdevinfo = UniqueHandleDevInfo(SetupDiGetClassDevsW(&GUID_DEVCLASS_BATTERY, //
                                                                 nullptr,                // Enumerator
                                                                 nullptr,                // hwndParent
                                                                 DIGCF_PRESENT | DIGCF_DEVICEINTERFACE));
        if (!hdevinfo) {
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
            auto hdev = UniqueHandleX(CreateFileW(didd.DevicePath,                      //
                                                  GENERIC_READ | GENERIC_WRITE,         //
                                                  FILE_SHARE_READ | FILE_SHARE_WRITE,   //
                                                  nullptr,                              // dwCreationDisposition
                                                  OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, //
                                                  nullptr));                            // hTemplateFile
            if (!hdev) {
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
                continue;
            }
            if (!battery_tag) {
                continue;
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
                continue;
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
                continue;
            }
        }
    }
} // namespace orange::battery
