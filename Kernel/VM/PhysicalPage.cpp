// includes
#include <Kernel/Heap/kmalloc.h>
#include <Kernel/VM/MemoryManager.h>
#include <Kernel/VM/PhysicalPage.h>

namespace Kernel {

NonnullRefPtr<PhysicalPage> PhysicalPage::create(PhysicalAddress paddr, bool supervisor, bool may_return_to_freelist)
{
    return adopt_ref(*new PhysicalPage(paddr, supervisor, may_return_to_freelist));
}

PhysicalPage::PhysicalPage(PhysicalAddress paddr, bool supervisor, bool may_return_to_freelist)
    : m_may_return_to_freelist(may_return_to_freelist)
    , m_supervisor(supervisor)
    , m_paddr(paddr)
{
}

void PhysicalPage::return_to_freelist() const
{
    VERIFY((paddr().get() & ~PAGE_MASK) == 0);

    if (m_supervisor)
        MM.deallocate_supervisor_physical_page(*this);
    else
        MM.deallocate_user_physical_page(*this);
}

}
