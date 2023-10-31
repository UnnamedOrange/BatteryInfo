#ifdef _WIN32

#include "battery_info_manager.h"

#include <Windows.h>
#include <batclass.h>
#include <devguid.h>

#include <setupapi.h> // setupapi.lib

using namespace orange::battery;
using Self = BatteryInfoManager;

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

auto Self::query_battery_info(uint32_t index) noexcept -> std::expected<BatteryInfo, ErrorType> {
    do {
        auto hdevinfo = UniqueHandleDevInfo(SetupDiGetClassDevsW(&GUID_DEVCLASS_BATTERY, //
                                                                 nullptr,                // Enumerator
                                                                 nullptr,                // hwndParent
                                                                 DIGCF_PRESENT | DIGCF_DEVICEINTERFACE));
        if (!hdevinfo) {
            break;
        }

        BOOL result;

        // Get interface data.
        auto did = SP_DEVICE_INTERFACE_DATA{
            .cbSize = sizeof(SP_DEVICE_INTERFACE_DATA),
        };
        result = SetupDiEnumDeviceInterfaces(hdevinfo,               //
                                             nullptr,                // DeviceInfoData
                                             &GUID_DEVCLASS_BATTERY, //
                                             index,                  //
                                             &did);
        if (!result) {
            const auto error = GetLastError();
            if (error == ERROR_NO_MORE_ITEMS) {
                return std::unexpected{Self::ErrorType::BATTERY_NOT_EXIST};
            } else {
                break;
            }
        }

        // Get device path.
        DWORD required_size;
        /* result = */ SetupDiGetDeviceInterfaceDetailW(hdevinfo, &did, nullptr, NULL, &required_size, nullptr);
        // Don't check result here. It's always false.
        auto buf = std::vector<std::byte>(required_size);
        auto& didd = *reinterpret_cast<SP_DEVICE_INTERFACE_DETAIL_DATA_W*>(buf.data());
        didd.cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA_W);
        result = SetupDiGetDeviceInterfaceDetailW(hdevinfo, &did, &didd, required_size, &required_size, nullptr);
        if (!result) {
            break;
        }

        // Open device.
        [[maybe_unused]] const auto _device_path = std::wstring_view{didd.DevicePath};
        auto hdev = UniqueHandleX(CreateFileW(didd.DevicePath,                      //
                                              GENERIC_READ | GENERIC_WRITE,         //
                                              FILE_SHARE_READ | FILE_SHARE_WRITE,   //
                                              nullptr,                              // dwCreationDisposition
                                              OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, //
                                              nullptr));                            // hTemplateFile
        if (!hdev) {
            break;
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
            break;
        }
        if (!battery_tag) {
            break;
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
            break;
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
            break;
        }

        const auto ret = BatteryInfo{
            .capacity = bs.Capacity,
            .power_state = bs.PowerState,
            .rate = bs.Rate,
            .voltage = bs.Voltage,

            .designed_capacity = bi.DesignedCapacity,
            .full_charged_capacity = bi.FullChargedCapacity,
            .default_alert_warning = bi.DefaultAlert1,
            .default_alert_low = bi.DefaultAlert2,

            .chemistry = std::string(bi.Chemistry, bi.Chemistry + 4),
            // .technology = bi.Technology,
            // .capability = bi.Capabilities,
            .critical_bias = bi.CriticalBias,
            .cycle_count = bi.CycleCount,
        };
        return ret;
    } while (false);
    return std::unexpected{Self::ErrorType::FAIL_TO_QUERY_INFORMATION};
}

#endif
