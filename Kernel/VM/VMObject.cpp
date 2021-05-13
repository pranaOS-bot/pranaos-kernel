// includes
#include <Kernel/VM/MemoryManager.h>
#include <Kernel/VM/VMObject.h>

namespace Kernel {

VMObject::VMObject(const VMObject& other)
    : m_physical_pages(other.m_physical_pages)
{
    MM.register_vmobject(*this);
}

VMObject::VMObject()
{
    MM.register_vmobject(*this);
}

VMObject::VMObject(size_t size)
{
    m_physical_pages.resize(ceil_div(size, static_cast<size_t>(PAGE_SIZE)));
    MM.register_vmobject(*this);
}

VMObject::~VMObject()
{
    {
        ScopedSpinLock lock(m_on_deleted_lock);
        for (auto& it : m_on_deleted)
            it->vmobject_deleted(*this);
        m_on_deleted.clear();
    }

    MM.unregister_vmobject(*this);
    VERIFY(m_regions_count.load(AK::MemoryOrder::memory_order_relaxed) == 0);
}

}
