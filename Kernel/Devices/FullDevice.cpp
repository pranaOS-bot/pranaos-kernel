// includes
#include "FullDevice.h"
#include <AK/Memory.h>
#include <AK/StdLibExtras.h>
#include <Kernel/Arch/x86/CPU.h>
#include <LibC/errno_numbers.h>

namespace Kernel {

UNMAP_AFTER_INIT FullDevice::FullDevice()
    : CharacterDevice(1, 7)
{
}

UNMAP_AFTER_INIT FullDevice::~FullDevice()
{
}

bool FullDevice::can_read(const FileDescription&, size_t) const
{
    return true;
}

KResultOr<size_t> FullDevice::read(FileDescription&, u64, UserOrKernelBuffer& buffer, size_t size)
{
    ssize_t count = min(static_cast<size_t>(PAGE_SIZE), size);
    if (!buffer.memset(0, count))
        return EFAULT;
    return count;
}

KResultOr<size_t> FullDevice::write(FileDescription&, u64, const UserOrKernelBuffer&, size_t size)
{
    if (size == 0)
        return 0;
    return ENOSPC;
}

}
