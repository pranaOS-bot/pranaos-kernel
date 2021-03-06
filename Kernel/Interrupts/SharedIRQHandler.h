#pragma once

// includes
#include <AK/HashTable.h>
#include <AK/NonnullOwnPtr.h>
#include <AK/RefPtr.h>
#include <AK/Types.h>
#include <Kernel/Arch/x86/CPU.h>
#include <Kernel/Interrupts/GenericInterruptHandler.h>

namespace Kernel {
class IRQHandler;
class SharedIRQHandler final : public GenericInterruptHandler {
public:
    static void initialize(u8 interrupt_number);
    virtual ~SharedIRQHandler();
    virtual void handle_interrupt(const RegisterState& regs) override;

    void register_handler(GenericInterruptHandler&);
    void unregister_handler(GenericInterruptHandler&);

    virtual bool eoi() override;

    virtual size_t sharing_devices_count() const override { return m_handlers.size(); }
    virtual bool is_shared_handler() const override { return true; }
    virtual bool is_sharing_with_others() const override { return false; }

    virtual HandlerType type() const override { return HandlerType::SharedIRQHandler; }
    virtual const char* purpose() const override { return "Shared IRQ Handler"; }
    virtual const char* controller() const override { return m_responsible_irq_controller->model(); }

private:
    void enable_interrupt_vector();
    void disable_interrupt_vector();
    explicit SharedIRQHandler(u8 interrupt_number);
    bool m_enabled { true };
    HashTable<GenericInterruptHandler*> m_handlers;
    RefPtr<IRQController> m_responsible_irq_controller;
};
}
