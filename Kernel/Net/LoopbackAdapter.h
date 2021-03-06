#pragma once

// includes
#include <Kernel/Net/NetworkAdapter.h>

namespace Kernel {

class LoopbackAdapter final : public NetworkAdapter {
    AK_MAKE_ETERNAL
public:
    LoopbackAdapter();
    static LoopbackAdapter& the();

    virtual ~LoopbackAdapter() override;

    virtual void send_raw(ReadonlyBytes) override;
    virtual const char* class_name() const override { return "LoopbackAdapter"; }
};

}
