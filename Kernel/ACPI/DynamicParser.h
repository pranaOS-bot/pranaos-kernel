// includes
#pragma once

#include <AK/RefPtr.h>
#include <Kernel/ACPI/Parser.h>
#include <Kernel/Interrupts/IRQHandler.h>
#include <Kernel/Lock.h>
#include <Kernel/PhysicalAddress.h>
#include <Kernel/VM/PhysicalPage.h>

namespace Kernel {
namespace ACPI {

class DynamicParser final
    : public IRQHandler
    , public Parser {
    friend class Parser;

public:
    virtual void enable_aml_interpretation() override;
    virtual void enable_aml_interpretation(File& dsdt_file) override;
    virtual void enable_aml_interpretation(u8* physical_dsdt, u32 dsdt_payload_legnth) override;
    virtual void disable_aml_interpretation() override;
    virtual void try_acpi_shutdown() override;
    virtual bool can_shutdown() override { return true; }
    virtual const char* purpose() const override { return "ACPI Parser"; }

protected:
    explicit DynamicParser(PhysicalAddress rsdp);

private:
    void build_namespace();
    // ^IRQHandler
    virtual void handle_irq(const RegisterState&) override;

    OwnPtr<Region> m_acpi_namespace;
};
}
}
