#include "ProcessGroup.h"

namespace Kernel {

RecursiveSpinLock g_process_groups_lock;
InlineLinkedList<ProcessGroup>* g_process_groups;

ProcessGroup::~ProcessGroup()
{
    ScopedSpinLock lock(g_process_groups_lock);
    g_process_groups->remove(this);
}

NonnullRefPtr<ProcessGroup> ProcessGroup::create(ProcessGroupID pgid)
{
    auto process_group = adopt(*new ProcessGroup(pgid));
    {
        ScopedSpinLock lock(g_process_groups_lock);
        g_process_groups->prepend(process_group);
    }

    return process_group;
}

NonnullRefPtr<ProcessGroup> ProcessGroup::find_or_create(ProcessGroupID pgid)
{
    ScopedSpinLock lock(g_process_groups_lock);

    if (auto existing = from_pgid(pgid))
        return existing.release_nonnull();

    return create(pgid);
}

RefPtr<ProcessGroup> ProcessGroup::from_pgid(ProcessGroupID pgid)
{
    ScopedSpinLock lock(g_process_groups_lock);

    for (auto& group : *g_process_groups) {
        if (group.pgid() == pgid)
            return &group;
    }
    return nullptr;
}

}
