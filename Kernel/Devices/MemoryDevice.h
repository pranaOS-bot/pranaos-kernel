#pragma once

// includes
#include <AK/String.h>
#include <AK/Types.h>
#include <Kernel/Devices/CharacterDevice.h>
#include <Kernel/PhysicalAddress.h>

namespace Kernel {

class MemoryDevice final : public CharacterDevice {
    AK_MAKE_ETERNAL
public:
    MemoryDevice();
    ~MemoryDevice();

    virtual KResultOr<Region*> mmap(Process&, FileDescription&, const Range&, u64 offset, int prot, bool shared) override;

    // ^Device
    virtual mode_t required_mode() const override { return 0660; }
    virtual String device_name() const override { return "mem"; };

private:
    virtual const char* class_name() const override { return "MemoryDevice"; }
    virtual bool can_read(const FileDescription&, size_t) const override { return true; }
    virtual bool can_write(const FileDescription&, size_t) const override { return false; }
    virtual bool is_seekable() const { return true; }
    virtual KResultOr<size_t> read(FileDescription&, u64, UserOrKernelBuffer&, size_t) override;
    virtual KResultOr<size_t> write(FileDescription&, u64, const UserOrKernelBuffer&, size_t) override { return -EINVAL; }

    virtual void did_seek(FileDescription&, off_t) override;

    bool is_allowed_range(PhysicalAddress, const Range&) const;
};

}
