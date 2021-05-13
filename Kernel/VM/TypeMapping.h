#pragma once

// includes
#include <AK/StringView.h>
#include <Kernel/VM/MemoryManager.h>

namespace Kernel {

template<typename T>
struct TypedMapping {
    const T* ptr() const { return reinterpret_cast<const T*>(region->vaddr().offset(offset).as_ptr()); }
    T* ptr() { return reinterpret_cast<T*>(region->vaddr().offset(offset).as_ptr()); }
    const T* operator->() const { return ptr(); }
    T* operator->() { return ptr(); }
    const T& operator*() const { return *ptr(); }
    T& operator*() { return *ptr(); }
    OwnPtr<Region> region;
    size_t offset { 0 };
};

template<typename T>
static TypedMapping<T> map_typed(PhysicalAddress paddr, size_t length, Region::Access access = Region::Access::Read)
{
    TypedMapping<T> table;
    table.region = MM.allocate_kernel_region(paddr.page_base(), page_round_up(length), {}, access);
    table.offset = paddr.offset_in_page();
    return table;
}

template<typename T>
static TypedMapping<T> map_typed(PhysicalAddress paddr)
{
    return map_typed<T>(paddr, sizeof(T));
}

template<typename T>
static TypedMapping<T> map_typed_writable(PhysicalAddress paddr)
{
    return map_typed<T>(paddr, sizeof(T), Region::Access::Read | Region::Access::Write);
}

}