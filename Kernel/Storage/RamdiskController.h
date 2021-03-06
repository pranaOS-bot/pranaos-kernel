#pragma once

// includes
#include <AK/OwnPtr.h>
#include <AK/RefPtr.h>
#include <AK/Types.h>
#include <Kernel/Storage/RamdiskDevice.h>
#include <Kernel/Storage/StorageController.h>
#include <Kernel/Storage/StorageDevice.h>

namespace Kernel {

class AsyncBlockDeviceRequest;

class RamdiskController final : public StorageController {
    AK_MAKE_ETERNAL
public:
public:
    static NonnullRefPtr<RamdiskController> initialize();
    virtual ~RamdiskController() override;

    virtual RefPtr<StorageDevice> device(u32 index) const override;
    virtual bool reset() override;
    virtual bool shutdown() override;
    virtual size_t devices_count() const override;
    virtual void start_request(const StorageDevice&, AsyncBlockDeviceRequest&) override;
    virtual void complete_current_request(AsyncDeviceRequest::RequestResult) override;

private:
    RamdiskController();

    NonnullRefPtrVector<RamdiskDevice> m_devices;
};
}
