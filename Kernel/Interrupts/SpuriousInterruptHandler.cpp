// includes
#include <Kernel/Interrupts/InterruptManagement.h>
#include <Kernel/Interrupts/SpuriousInterruptHandler.h>

namespace Kernel {

UNMAP_AFTER_INIT void SpuriousInterruptHandler::initialize(u8 interrupt_number)
{
    auto* handler = new SpuriousInterruptHandler(interrupt_number);
    handler->register_interrupt_handler();
}

void SpuriousInterruptHandler::register_handler(GenericInterruptHandler& handler)
{
    VERIFY(!m_real_handler);
    m_real_handler = &handler;
}
void SpuriousInterruptHandler::unregister_handler(GenericInterruptHandler&)
{
    TODO();
}

bool SpuriousInterruptHandler::eoi()
{
    // Actually check if IRQ7 or IRQ15 are spurious, and if not, call EOI with the correct interrupt number.
    if (m_real_irq) {
        m_responsible_irq_controller->eoi(*this);
        m_real_irq = false; // return to default state!
        return true;
    }
    m_responsible_irq_controller->spurious_eoi(*this);
    return false;
}

const char* SpuriousInterruptHandler::purpose() const
{
    if (!m_real_handler)
        return "Spurious Interrupt Handler";
    return m_real_handler->purpose();
}

SpuriousInterruptHandler::SpuriousInterruptHandler(u8 irq)
    : GenericInterruptHandler(irq)
    , m_responsible_irq_controller(InterruptManagement::the().get_responsible_irq_controller(irq))
{
}

SpuriousInterruptHandler::~SpuriousInterruptHandler()
{
}

void SpuriousInterruptHandler::handle_interrupt(const RegisterState& state)
{
    // Actually check if IRQ7 or IRQ15 are spurious, and if not, call the real handler to handle the IRQ.
    if (m_responsible_irq_controller->get_isr() & (1 << interrupt_number())) {
        m_real_irq = true; // remember that we had a real IRQ, when EOI later!
        m_real_handler->increment_invoking_counter();
        m_real_handler->handle_interrupt(state);
        return;
    }
    dbgln("Spurious interrupt, vector {}", interrupt_number());
}

void SpuriousInterruptHandler::enable_interrupt_vector()
{
    if (m_enabled)
        return;
    m_enabled = true;
    m_responsible_irq_controller->enable(*this);
}

void SpuriousInterruptHandler::disable_interrupt_vector()
{
    VERIFY(!m_real_irq); // this flag should not be set when we call this method
    if (!m_enabled)
        return;
    m_enabled = false;
    m_responsible_irq_controller->disable(*this);
}

const char* SpuriousInterruptHandler::controller() const
{
    if (m_responsible_irq_controller->type() == IRQControllerType::i82093AA)
        return "";
    return m_responsible_irq_controller->model();
}
}
