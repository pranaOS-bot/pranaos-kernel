// includes
#include <AK/Singleton.h>
#include <Kernel/Net/LoopbackAdapter.h>

namespace Kernel {

static AK::Singleton<LoopbackAdapter> s_loopback;

LoopbackAdapter& LoopbackAdapter::the()
{
    return *s_loopback;
}

LoopbackAdapter::LoopbackAdapter()
{
    set_interface_name("loop");
    set_mtu(65536);
    set_mac_address({ 19, 85, 2, 9, 0x55, 0xaa });
}

LoopbackAdapter::~LoopbackAdapter()
{
}

void LoopbackAdapter::send_raw(ReadonlyBytes payload)
{
    dbgln("LoopbackAdapter: Sending {} byte(s) to myself.", payload.size());
    did_receive(payload);
}

}
