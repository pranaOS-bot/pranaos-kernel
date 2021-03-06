#pragma once

// includes
#include <Kernel/Lock.h>
#include <Kernel/Storage/StorageDevice.h>

namespace Kernel {

class RamdiskController;

class RamdiskDevice final : public StorageDevice {
    friend class RamdiskController;
    AK_MAKE_ETERNAL
public:
    static NonnullRefPtr<RamdiskDevice> create(const RamdiskController&, NonnullOwnPtr<Region>&& region, int major, int minor);
    RamdiskDevice(const RamdiskController&, NonnullOwnPtr<Region>&&, int major, int minor);
    virtual ~RamdiskDevice() override;

    // ^BlockDevice
    virtual void start_request(AsyncBlockDeviceRequest&) override;

    // ^DiskDevice
    virtual const char* class_name() const override;
    virtual String device_name() const override;

    bool is_slave() const;

    Lock m_lock { "RamdiskDevice" };

    NonnullOwnPtr<Region> m_region;
};

}
