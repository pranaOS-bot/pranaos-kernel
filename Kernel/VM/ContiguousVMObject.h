#pragma once

// includes
#include <Kernel/PhysicalAddress.h>
#include <Kernel/VM/VMObject.h>

namespace Kernel {

class ContiguousVMObject final : public VMObject {
public:
    virtual ~ContiguousVMObject() override;

    static NonnullRefPtr<ContiguousVMObject> create_with_size(size_t, size_t physical_alignment = PAGE_SIZE);

private:
    explicit ContiguousVMObject(size_t, size_t physical_alignment);
    explicit ContiguousVMObject(const ContiguousVMObject&);

    virtual const char* class_name() const override { return "ContiguousVMObject"; }
    virtual RefPtr<VMObject> clone() override;

    ContiguousVMObject& operator=(const ContiguousVMObject&) = delete;
    ContiguousVMObject& operator=(ContiguousVMObject&&) = delete;
    ContiguousVMObject(ContiguousVMObject&&) = delete;

    virtual bool is_contiguous() const override { return true; }
};

}
