#pragma once

// includes
#include <AK/RefPtr.h>
#include <AK/Vector.h>
#include <Kernel/Storage/Partition/DiskPartition.h>
#include <Kernel/Storage/Partition/DiskPartitionMetadata.h>
#include <Kernel/Storage/StorageDevice.h>

namespace Kernel {

class PartitionTable {
public:
    enum class Error {
        Invalid,
        MBRProtective,
        ConatinsEBR,
    };

public:
    Optional<DiskPartitionMetadata> partition(unsigned index);
    size_t partitions_count() const { return m_partitions.size(); }
    virtual ~PartitionTable() = default;
    virtual bool is_valid() const = 0;

    Vector<DiskPartitionMetadata> partitions() const { return m_partitions; }

protected:
    explicit PartitionTable(const StorageDevice&);

    NonnullRefPtr<StorageDevice> m_device;
    Vector<DiskPartitionMetadata> m_partitions;
};

}
