#pragma once

// includes
#include <AK/CircularQueue.h>
#include <AK/DoublyLinkedList.h>
#include <AK/Types.h>
#include <Kernel/API/KeyCode.h>
#include <Kernel/API/MousePacket.h>
#include <Kernel/Devices/CharacterDevice.h>
#include <Kernel/Devices/HID/HIDDevice.h>
#include <Kernel/Interrupts/IRQHandler.h>
#include <Kernel/Random.h>

namespace Kernel {

class MouseDevice : public HIDDevice {
public:
    virtual ~MouseDevice() override;

    // ^CharacterDevice
    virtual KResultOr<size_t> read(FileDescription&, u64, UserOrKernelBuffer&, size_t) override;
    virtual bool can_read(const FileDescription&, size_t) const override;
    virtual KResultOr<size_t> write(FileDescription&, u64, const UserOrKernelBuffer&, size_t) override;
    virtual bool can_write(const FileDescription&, size_t) const override { return true; }

    // ^HIDDevice
    virtual Type instrument_type() const { return Type::Mouse; }

    // ^Device
    virtual mode_t required_mode() const override { return 0440; }

    //FIXME: It should be something like String::formatted("mouse{}", minor())
    // instead of a fixed string like this
    virtual String device_name() const override { return "mouse"; }

protected:
    MouseDevice();
    // ^CharacterDevice
    virtual const char* class_name() const override { return "MouseDevice"; }

    mutable SpinLock<u8> m_queue_lock;
    CircularQueue<MousePacket, 100> m_queue;
};

}
