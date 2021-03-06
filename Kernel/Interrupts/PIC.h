#pragma once

// includes
#include <AK/Types.h>
#include <Kernel/Interrupts/IRQController.h>

namespace Kernel {
class PIC final : public IRQController {
public:
    PIC();
    virtual void enable(const GenericInterruptHandler&) override;
    virtual void disable(const GenericInterruptHandler&) override;
    virtual void hard_disable() override;
    virtual void eoi(const GenericInterruptHandler&) const override;
    virtual bool is_vector_enabled(u8 number) const override;
    virtual bool is_enabled() const override;
    virtual void spurious_eoi(const GenericInterruptHandler&) const override;
    virtual u16 get_isr() const override;
    virtual u16 get_irr() const override;
    virtual u32 gsi_base() const override { return 0; }
    virtual size_t interrupt_vectors_count() const { return 16; }
    virtual const char* model() const override { return "Dual i8259"; }
    virtual IRQControllerType type() const override { return IRQControllerType::i8259; }

private:
    u16 m_cached_irq_mask { 0xffff };
    void eoi_interrupt(u8 irq) const;
    void enable_vector(u8 number);
    void remap(u8 offset);
    void complete_eoi() const;
    virtual void initialize() override;
};

}
