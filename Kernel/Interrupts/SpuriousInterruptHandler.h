#pragma once

// includes
#include <AK/OwnPtr.h>
#include <AK/Types.h>
#include <Kernel/Arch/x86/CPU.h>
#include <Kernel/Interrupts/GenericInterruptHandler.h>
#include <Kernel/Interrupts/IRQController.h>

namespace Kernel {

class SpuriousInterruptHandler final : public GenericInterruptHandler {
public:
    static void initialize(u8 interrupt_number);
    virtual ~SpuriousInterruptHandler();
    virtual void handle_interrupt(const RegisterState& regs) override;

    void register_handler(GenericInterruptHandler&);
    void unregister_handler(GenericInterruptHandler&);

    virtual bool eoi() override;

    virtual size_t sharing_devices_count() const override { return 1; }
    virtual bool is_shared_handler() const override { return false; }
    virtual bool is_sharing_with_others() const override { return false; }

    virtual HandlerType type() const override { return HandlerType::SpuriousInterruptHandler; }
    virtual const char* purpose() const override;
    virtual const char* controller() const override;

private:
    void enable_interrupt_vector();
    void disable_interrupt_vector();
    explicit SpuriousInterruptHandler(u8 interrupt_number);
    bool m_enabled;
    bool m_real_irq { false };
    RefPtr<IRQController> m_responsible_irq_controller;
    OwnPtr<GenericInterruptHandler> m_real_handler;
};
}
