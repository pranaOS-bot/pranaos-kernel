#pragma once

// includes
#include <AK/NonnullOwnPtr.h>
#include <AK/RefPtr.h>
#include <AK/Result.h>
#include <AK/Vector.h>
#include <Kernel/Storage/Partition/DiskPartition.h>
#include <Kernel/Storage/Partition/MBRPartitionTable.h>

namespace Kernel {

struct EBRPartitionHeader;
class EBRPartitionTable : public MBRPartitionTable {
public:
    ~EBRPartitionTable();

    static Result<NonnullOwnPtr<EBRPartitionTable>, PartitionTable::Error> try_to_initialize(const StorageDevice&);
    explicit EBRPartitionTable(const StorageDevice&);
    virtual bool is_valid() const override { return m_valid; };

private:
    void search_extended_partition(const StorageDevice&, MBRPartitionTable&, u64, size_t limit);

    bool m_valid { false };
};
}
