// includes
#include <Kernel/Debug.h>
#include <Kernel/IO.h>
#include <Kernel/PCI/IOAccess.h>

namespace Kernel {
namespace PCI {

UNMAP_AFTER_INIT void IOAccess::initialize()
{
    if (!Access::is_initialized()) {
        new IOAccess();
        dbgln_if(PCI_DEBUG, "PCI: IO access initialised.");
    }
}

UNMAP_AFTER_INIT IOAccess::IOAccess()
{
    dmesgln("PCI: Using I/O instructions for PCI configuration space access");
    enumerate_hardware([&](const Address& address, ID id) {
        m_physical_ids.append({ address, id, get_capabilities(address) });
    });
}

u8 IOAccess::read8_field(Address address, u32 field)
{
    dbgln_if(PCI_DEBUG, "PCI: IO Reading 8-bit field {:#08x} for {}", field, address);
    return Access::early_read8_field(address, field);
}

u16 IOAccess::read16_field(Address address, u32 field)
{
    dbgln_if(PCI_DEBUG, "PCI: IO Reading 16-bit field {:#08x} for {}", field, address);
    return Access::early_read16_field(address, field);
}

u32 IOAccess::read32_field(Address address, u32 field)
{
    dbgln_if(PCI_DEBUG, "PCI: IO Reading 32-bit field {:#08x} for {}", field, address);
    return Access::early_read32_field(address, field);
}

void IOAccess::write8_field(Address address, u32 field, u8 value)
{
    dbgln_if(PCI_DEBUG, "PCI: IO Writing to 8-bit field {:#08x}, value={:#02x} for {}", field, value, address);
    IO::out32(PCI_ADDRESS_PORT, address.io_address_for_field(field));
    IO::out8(PCI_VALUE_PORT + (field & 3), value);
}
void IOAccess::write16_field(Address address, u32 field, u16 value)
{
    dbgln_if(PCI_DEBUG, "PCI: IO Writing to 16-bit field {:#08x}, value={:#02x} for {}", field, value, address);
    IO::out32(PCI_ADDRESS_PORT, address.io_address_for_field(field));
    IO::out16(PCI_VALUE_PORT + (field & 2), value);
}
void IOAccess::write32_field(Address address, u32 field, u32 value)
{
    dbgln_if(PCI_DEBUG, "PCI: IO Writing to 32-bit field {:#08x}, value={:#02x} for {}", field, value, address);
    IO::out32(PCI_ADDRESS_PORT, address.io_address_for_field(field));
    IO::out32(PCI_VALUE_PORT, value);
}

void IOAccess::enumerate_hardware(Function<void(Address, ID)> callback)
{
    dbgln_if(PCI_DEBUG, "PCI: IO enumerating hardware");
    // Single PCI host controller.
    if ((read8_field(Address(), PCI_HEADER_TYPE) & 0x80) == 0) {
        enumerate_bus(-1, 0, callback, true);
        return;
    }

    // Multiple PCI host controllers.
    for (int bus = 0; bus < 256; ++bus) {
        if (read16_field(Address(0, 0, 0, bus), PCI_VENDOR_ID) == PCI_NONE)
            break;
        enumerate_bus(-1, bus, callback, false);
    }
}

}
}
